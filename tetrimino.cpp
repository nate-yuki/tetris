#include "tetrimino.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <fstream>
#include <algorithm>


TetriminoConfig::TetriminoConfig ()
    : type(Tetrimino::TetriminoType(rand() % Tetrimino::TETRIMINO_TOTAL))
    , rot(Tetrimino::TetriminoRotation(rand() % Tetrimino::TETRIMINO_ROTATION_TOTAL))
{}

TetriminoConfig::TetriminoConfig(
    Tetrimino::TetriminoType type, Tetrimino::TetriminoRotation rot
)
    : type(type)
    , rot(rot)
{}


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

void Tetrimino::init (TetrisField *field, Texture *blockTextureSheet)
{
    this->field = field;
    this->blockTextureSheet = blockTextureSheet;
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
    bool fit = true;

    rotations = &schemes[config.type];

    totalBlocks = 0;
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if (schemes[config.type][config.rot][row][col])
            {
                if (field->has_block(posX + col, posY + row))
                {
                    fit = false;
                }
                
                SDL_Rect *clip =
                    ghost ? &blockClips[TETRIMINO_TOTAL] : &blockClips[config.type];
                blocks.push_back(new Block(blockTextureSheet, clip));
                ++totalBlocks;
            }
        }
    }
    fallElapsed = sideElapsed = rotElapsed = 0;
    sideVel = rotVel = 0;
    
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
    {
        sideVel += TETRIMINO_SIDE_SPEED;
    }
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
    {
        sideVel -= TETRIMINO_SIDE_SPEED;
    }
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S])
    {
        fallDelay /= TETRIMINO_DROP_ACC;
    }
    if (keys[SDL_SCANCODE_Q])
    {
        rotVel += TETRIMINO_ROT_SPEED;
    }
    if (keys[SDL_SCANCODE_E])
    {
        rotVel -= TETRIMINO_ROT_SPEED;
    }
    
    this->posX = posX;
    this->posY = posY;
    this->fallDelay = fallDelay;
    this->type = config.type;
    this->rot = config.rot;

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
    if (!check_collision_bottom())
    {
        Tetrimino ghost;
        ghost.init(field, blockTextureSheet);
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
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
        case SDLK_d:
            shift(1);
            sideVel += TETRIMINO_SIDE_SPEED;
            break;
        case SDLK_LEFT:
        case SDLK_a:
            shift(-1);
            sideVel -= TETRIMINO_SIDE_SPEED;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            fallDelay /= TETRIMINO_DROP_ACC;
            break;
        case SDLK_UP:
        case SDLK_w:
            drop();
            stop();
            break;
        case SDLK_q:
            rotate(1);
            rotVel += TETRIMINO_ROT_SPEED;
            break;
        case SDLK_e:
            rotate(-1);
            rotVel -= TETRIMINO_ROT_SPEED;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
        case SDLK_d:
            sideVel -= TETRIMINO_SIDE_SPEED;
            break;
        case SDLK_LEFT:
        case SDLK_a:
            sideVel += TETRIMINO_SIDE_SPEED;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            fallDelay *= TETRIMINO_DROP_ACC;
            break;
        case SDLK_q:
            rotVel -= TETRIMINO_ROT_SPEED;
            break;
        case SDLK_e:
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
            return true;
        }
        ++posY;
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
        sideElapsed = sideVel * dt;
    }
    else
    {
        sideElapsed += sideVel * dt;
    }
    if (sideElapsed / 1000)
    {
        shift(sideElapsed / 1000);
        sideElapsed -= 1000 * sideVel / abs(sideVel);
    }

    if (rotElapsed * rotVel <= 0)
    {
        rotElapsed = rotVel * dt;
    }
    else
    {
        rotElapsed += rotVel * dt;
    }
    if (rotElapsed / 1000)
    {
        rotate(rotElapsed / 1000);
        rotElapsed -= 1000 * rotVel / abs(rotVel);
    }
}

TetriminoConfig Tetrimino::get_config() const
{
    return TetriminoConfig(type, rot);
}

void Tetrimino::shift (int dx)
{
    posX += dx;
    if (dx > 0 && check_collision_right() || dx < 0 && check_collision_left())
    {
        posX -= dx;
    }
}

void Tetrimino::drop()
{
    while (!check_collision_bottom())
    {
        ++posY;
    }
}

void Tetrimino::rotate (int dir)
{
    int newRot = (rot + dir + TETRIMINO_ROTATION_TOTAL) % TETRIMINO_ROTATION_TOTAL;
    int col;
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[newRot][row][col])
            {
                if (
                    posX + col < 0 || posX + col >= TETRIS_FIELD_WIDTH ||
                    posY + row >= field->get_height() ||
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
        rot = TetriminoRotation(newRot);
    }
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
                    posX + col >= TETRIS_FIELD_WIDTH ||
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
