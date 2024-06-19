#include "tetrimino.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <fstream>
#include <algorithm>


std::vector<std::vector<Scheme>> Tetrimino::schemes;
std::vector<SDL_Rect> Tetrimino::blockClips;

#include <iostream>
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
    for (int type = 0; type < TETRIMINO_TOTAL; ++type)
    {
        blockClips.push_back(
            {type * MAX_BLOCK_SIZE, 0, MAX_BLOCK_SIZE, MAX_BLOCK_SIZE}
        );
    }
}

void Tetrimino::init (
    int posX, int posY, int moveDelay, Texture &blockTextureSheet, TetriminoType type,
    TetriminoRotation rot
)
{
    rotations = &schemes[type];

    totalBlocks = 0;
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if (schemes[type][rot][row][col])
            {
                ++totalBlocks;
                blocks.push_back(new Block(&blockTextureSheet, &blockClips[type]));
            }
        }
    }
    this->posX = posX;
    this->posY = posY;
    this->moveDelay = moveDelay;
    this->elapsed = 0;
    this->type = type;
    this->rot = rot;
}

void Tetrimino::render(int x, int y, int size)
{
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
}

bool Tetrimino::move (TetrisField &field, int dt)
{
    elapsed += dt;
    if (elapsed >= moveDelay)
    {
        elapsed -= moveDelay;
        if (check_collision(field))
        {
            stop(field);
            return true;
        }
        ++posY;
    }
    return false;
}

bool Tetrimino::check_collision (TetrisField &field)
{
    for (int row = MAX_SCHEME_LEN - 1; row >= 0; --row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[rot][row][col])
            {
                if (posY + row + 1 == field.get_height())
                {
                    return true;
                }
                if (field.has_block(posX + col, posY + row + 1))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void Tetrimino::stop (TetrisField &field)
{
    int blocksAdded = 0;
    for (int row = 0; row < MAX_SCHEME_LEN; ++row)
    {
        for (int col = 0; col < MAX_SCHEME_LEN; ++col)
        {
            if ((*rotations)[rot][row][col])
            {
                field.add_block(posX + col, posY + row, blocks[blocksAdded]);
                if (++blocksAdded == totalBlocks)
                {
                    row = MAX_SCHEME_LEN;
                    break;
                }
            }
        }
    }
    blocks.resize(0);
}
