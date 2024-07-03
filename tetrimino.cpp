/**
 * @file  tetrimino.cpp
 * @brief Implementation of the class Tetrimino and TetriminoConfig struct.
 */

#include "tetrimino.hpp"
#include "game.hpp"
#include "audio.hpp"
#include "key_layout.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <fstream>
#include <algorithm>


std::vector<std::vector<Scheme>> Tetrimino::schemes;
std::vector<SDL_Rect> Tetrimino::blockClips;


void Tetrimino::load_schemes (const std::string &path)
{
    std::ifstream fin(path);
    if (fin.fail())
    {
        std::string msg = "Could not open \"" + path + "\"";
        throw ExceptionFile(__FILE__, __LINE__, msg.c_str());
    }
    for (int type = 0; type < TETRIMINO_TOTAL; ++type)
    {
        schemes.push_back(std::vector<Scheme>{});
        for (int rot = 0; rot < TETRIMINO_ROTATION_TOTAL; ++rot)
        {
            schemes[type].push_back(Scheme{});
            for (int row = 0; row < MAX_SCHEME_LEN; ++row)
            {
                schemes[type][rot].push_back(SchemeRow{});
                for (int col = 0; col < MAX_SCHEME_LEN; ++col)
                {
                    SchemeElem elem;
                    fin >> elem;
                    if (fin.fail())
                    {
                        std::string msg = "Could not read from \"" + path + "\"";
                        throw ExceptionFile(__FILE__, __LINE__, msg.c_str());
                    }
                    
                    schemes[type][rot][row].push_back(elem);
                }
            }
        }
    }
    fin.close();
}

void Tetrimino::init_clips ()
{
    for (int type = 0; type < TETRIMINO_TOTAL + 1; ++type)
    {
        blockClips.push_back(
            {type * MAX_BLOCK_SIZE, 0, MAX_BLOCK_SIZE, MAX_BLOCK_SIZE}
        );
    }
}

void Tetrimino::render_config (
    const TetriminoConfig &config, int x, int y, int size, Texture *blockTextureSheet
)
{
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if (schemes[config.type][config.rot][row][col])
            {
                Block(blockTextureSheet, &blockClips[config.type]).render(
                    x + col * size, y + row * size, size
                );
            }
        }
    }
}

void Tetrimino::init (
    TetrisField *field, Texture *blockTextureSheet, KeyLayout *keyLayout
)
{
    this->field = field;
    this->blockTextureSheet = blockTextureSheet;
    this->keyLayout = keyLayout;
    totalBlocks = 0;
}

void Tetrimino::free ()
{
    log("Freeing Tetrimino", __FILE__, __LINE__);
    
    for (Block *block: blocks)
    {
        delete block;
    }
    blocks.resize(0);
}

bool Tetrimino::spawn (
    int posX, int posY, int fallDelay, const TetriminoConfig &config,
    bool ghost
)
{
    this->type = config.type;
    this->rot = config.rot;
    this->posX = posX;
    this->posY = posY;

    bool fit = true;

    rotations = &schemes[config.type];

    // Check if the tetrimino fits and create blocks
    SDL_Rect *clip = ghost ? &blockClips[TETRIMINO_TOTAL] : &blockClips[config.type];
    totalBlocks = 0;
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[config.rot][row][col])
            {
                if (field->has_block(posX + col, posY + row))
                {
                    fit = false;
                }
                
                blocks.push_back(new Block(blockTextureSheet, clip));
                ++totalBlocks;
            }
        }
    }

    // Only needed to create the blocks for a ghost
    if (ghost)
    {
        return true;
    }

    fallElapsed = sideElapsed = rotElapsed = 0;
    sideVel = rotVel = 0;
    
    // If movement control buttons are pressed, set corresponding velocities
    keyLayout->store_pressed();
    if (keyLayout->pressed(RIGHT))
    {
        sideVel += TETRIMINO_SIDE_SPEED;
    }
    if (keyLayout->pressed(LEFT))
    {
        sideVel -= TETRIMINO_SIDE_SPEED;
    }
    if (keyLayout->pressed(ACC))
    {
        fallDelay /= TETRIMINO_DROP_ACC;
    }
    if (keyLayout->pressed(ROT_CCW))
    {
        rotVel += TETRIMINO_ROT_SPEED;
    }
    if (keyLayout->pressed(ROT_CW))
    {
        rotVel -= TETRIMINO_ROT_SPEED;
    }

    this->fallDelay = fallDelay;

    return fit;
}

void Tetrimino::render (int x, int y, int size)
{
    if (!totalBlocks)
    {
        return;
    }
    int blocksRendered = 0;
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[rot][row][col])
            {
                blocks[blocksRendered]->render(
                    x + (posX + col) * size, y + (posY + row) * size, size
                );
                if (++blocksRendered == totalBlocks)
                {
                    row = MAX_SCHEME_LEN;
                    break;
                }
            }
        }
    }
    // Render a ghost
    if (!check_collision_bottom())
    {
        Tetrimino ghost;
        ghost.init(field, blockTextureSheet, nullptr);
        ghost.spawn(posX, posY, -1, TetriminoConfig(type, rot), true);
        ghost.drop();
        ghost.render(x, y, size);
    }
}

void Tetrimino::handle_event (Game &game, const SDL_Event &e)
{
    if (!totalBlocks)
    {
        return;
    }
    keyLayout->handle_event(game, e);
    if (keyLayout->get_type() == KeyLayout::DOWN && keyLayout->get_repeat() == 0)
    {
        switch (keyLayout->get_command())
        {
        case RIGHT:
            if (!game.is_paused())
            {
                shift(1);
            }
            sideVel += TETRIMINO_SIDE_SPEED;
            break;
        case LEFT:
            if (!game.is_paused())
            {
                shift(-1);
            }
            sideVel -= TETRIMINO_SIDE_SPEED;
            break;
        case ACC:
            fallDelay /= TETRIMINO_DROP_ACC;
            if (fallElapsed > fallDelay)
            {
                // Decrease elapsed falling time so that the tetrimino does not
                // bolt down
                fallElapsed = fallDelay;
            }
            break;
        case DROP:
            if (!game.is_paused())
            {
                drop();
                stop();

                Audio::play_sound(Audio::TETRIMINO_DROP);
            }
            break;
        case ROT_CCW:
            if (!game.is_paused())
            {
                if (rotate(1))
                {
                    Audio::play_sound(Audio::TETRIMINO_ROTATE);
                }
                else
                {
                    Audio::play_sound(Audio::TETRIMINO_BLOCKED);
                }
            }
            rotVel += TETRIMINO_ROT_SPEED;
            break;
        case ROT_CW:
            if (!game.is_paused())
            {
                if (rotate(-1))
                {
                    Audio::play_sound(Audio::TETRIMINO_ROTATE);
                }
                else
                {
                    Audio::play_sound(Audio::TETRIMINO_BLOCKED);
                }
            }
            rotVel -= TETRIMINO_ROT_SPEED;
            break;
        }
    }
    else if (keyLayout->get_type() == KeyLayout::UP && keyLayout->get_repeat() == 0)
    {
        switch (keyLayout->get_command())
        {
        case RIGHT:
            sideVel -= TETRIMINO_SIDE_SPEED;
            break;
        case LEFT:
            sideVel += TETRIMINO_SIDE_SPEED;
            break;
        case ACC:
            fallDelay *= TETRIMINO_DROP_ACC;
            fallElapsed *= TETRIMINO_DROP_ACC;
            break;
        case ROT_CCW:
            rotVel -= TETRIMINO_ROT_SPEED;
            break;
        case ROT_CW:
            rotVel += TETRIMINO_ROT_SPEED;
            break;
        }
    }
}

bool Tetrimino::fall (int dt)
{
    if (!totalBlocks)
    {
        return true;
    }
    fallElapsed += dt;
    if (fallElapsed >= fallDelay)
    {
        fallElapsed -= fallDelay;
        if (check_collision_bottom())
        {
            stop();

            Audio::play_sound(Audio::TETRIMINO_STOP);
            return true;
        }
        ++posY;
            
        Audio::play_sound(Audio::TETRIMINO_FALL);
    }
    return false;
}

void Tetrimino::move (int dt)
{
    if (!totalBlocks)
    {
        return;
    }

    if (sideElapsed * sideVel <= 0)
    {
        // If velocity changed direction, reset the elapsed time
        sideElapsed = sideVel * dt;
    }
    else
    {
        // Otherwise, accumulate the elapsed time
        sideElapsed += sideVel * dt;
    }
    if (sideElapsed / 1000)
    {
        shift(sideElapsed / 1000);
        sideElapsed -= 1000 * sideVel / abs(sideVel);
    }

    if (rotElapsed * rotVel <= 0)
    {
        // If velocity changed direction, reset the elapsed time
        rotElapsed = rotVel * dt;
    }
    else
    {
        // Otherwise, accumulate the elapsed time
        rotElapsed += rotVel * dt;
    }
    if (rotElapsed / 1000)
    {
        if (rotate(rotElapsed / 1000))
        {
            Audio::play_sound(Audio::TETRIMINO_ROTATE);
        }
        else
        {
            Audio::play_sound(Audio::TETRIMINO_BLOCKED);
        }
        rotElapsed -= 1000 * rotVel / abs(rotVel);
    }
}

TetriminoConfig Tetrimino::get_config () const
{
    return TetriminoConfig(type, rot);
}

void Tetrimino::shift (int dx)
{
    posX += dx;
    if (dx > 0 && check_collision_right() || dx < 0 && check_collision_left())
    {
        posX -= dx;

        Audio::play_sound(Audio::TETRIMINO_BLOCKED);
    }
    else
    {
        Audio::play_sound(Audio::TETRIMINO_MOVE);
    }
}

void Tetrimino::drop ()
{
    while (!check_collision_bottom())
    {
        ++posY;
    }
}

bool Tetrimino::check_adjacent (int dir, int dx, int dy)
{
    posX += dx;
    posY += dy;
    if (rotate(dir, false))
    {
        return true;
    }
    posX -= dx;
    posY -= dy;
    return false;
}

bool Tetrimino::rotate (int dir, bool checkAdjacent)
{
    int newRot = (rot + dir + TETRIMINO_ROTATION_TOTAL) % TETRIMINO_ROTATION_TOTAL;
    int col;

    // Check wether rotating creates a collision
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[newRot][row][col])
            {
                if (
                    posX + col < 0 || posX + col >= field->get_width() ||
                    posY + row < 0 || posY + row >= field->get_height() ||
                    field->has_block(posX + col, posY + row)
                )
                {
                    row = MAX_SCHEME_LEN;
                    break;
                }
            }
        }
    }
    if (col == MAX_SCHEME_LEN)
    {
        // No collisions
        rot = TetriminoRotation(newRot);
        return true;
    }
    if (checkAdjacent)
    {
        // If there was a collision, try shifting no more than twice
        if (
            check_adjacent(dir, -1, 0) || check_adjacent(dir, 1, 0) ||
            check_adjacent(dir, 0, -1) || check_adjacent(dir, 0, 1) ||
            check_adjacent(dir, -2, 0) || check_adjacent(dir, 2, 0) ||
            check_adjacent(dir, 0, -2) || check_adjacent(dir, 0, 2) ||
            check_adjacent(dir, -1, -1) || check_adjacent(dir, 1, -1) ||
            check_adjacent(dir, -1, 1) || check_adjacent(dir, 1, 1)
        )
        {
            return true;
        }
    }
    return false;
}

bool Tetrimino::check_collision_left ()
{
    for (int col = 0; col < MAX_SCHEME_LEN; ++col)
    {
        for (int row = 0; row < MAX_SCHEME_LEN; ++row)
        {
            if ((*rotations)[rot][row][col])
            {
                if (posX + col < 0 || field->has_block(posX + col, posY + row))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Tetrimino::check_collision_right ()
{
    for (int col = MAX_SCHEME_LEN - 1; col >= 0; --col)
    {
        for (int row = 0; row < MAX_SCHEME_LEN; ++row)
        {
            if ((*rotations)[rot][row][col])
            {
                if (
                    posX + col >= field->get_width() ||
                    field->has_block(posX + col, posY + row)
                )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Tetrimino::check_collision_bottom ()
{
    for (int row = MAX_SCHEME_LEN - 1; row >= 0; --row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[rot][row][col])
            {
                if (
                    posY + row + 1 >= field->get_height() ||
                    field->has_block(posX + col, posY + row + 1)
                )
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void Tetrimino::stop ()
{
    int blocksAdded = 0;
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[rot][row][col])
            {
                field->add_block(posX + col, posY + row, blocks[blocksAdded]);
                if (++blocksAdded == totalBlocks)
                {
                    row = MAX_SCHEME_LEN;
                    break;
                }
            }
        }
    }
    blocks.resize(0);
    totalBlocks = 0;
}


TetriminoConfig::TetriminoConfig ()
    : type(Tetrimino::TetriminoType(rand() % Tetrimino::TETRIMINO_TOTAL))
    , rot(Tetrimino::TetriminoRotation(rand() % Tetrimino::TETRIMINO_ROTATION_TOTAL))
{}

TetriminoConfig::TetriminoConfig (
    Tetrimino::TetriminoType type, Tetrimino::TetriminoRotation rot
)
    : type(type)
    , rot(rot)
{}
