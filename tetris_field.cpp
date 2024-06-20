#include "tetris_field.hpp"
#include "logger.hpp"


Block::Block (Texture *texture, const SDL_Rect *clip)
    : texture(texture)
    , clip(clip)
{}

void Block::render (int x, int y, int size)
{
    texture->render({x, y, size, size}, clip);
}


void TetrisField::init (int cellsHor, int cellsVer, Texture *bgTexture)
{
    field = std::vector<std::vector<Block *>>(
        cellsVer, std::vector<Block *>(cellsHor, nullptr)
    );
    this->cellsHor = cellsHor;
    this->cellsVer = cellsVer;
    this->bgTexture = bgTexture;
}

void TetrisField::free ()
{
    for (int row = 0; row < cellsVer; ++row)
    {
        for (int col = 0; col < cellsHor; ++col)
        {
            if (field[row][col] != nullptr)
            {
                delete field[row][col];
            }
        }
    }
}

void TetrisField::render (int x, int y, int maxW, int maxH)
{
    int size = min(maxW / cellsHor, maxH / cellsVer);
    for (int row = 0; row < cellsVer; ++row)
    {
        for (int col = 0; col < cellsHor; ++col)
        {
            if (field[row][col] != nullptr)
            {
                field[row][col]->render(x + col * size, y + row * size, size);
            }
            else
            {
                bgTexture->render({x + col * size, y + row * size, size, size});
            }
        }
    }
}

bool TetrisField::has_block (int posX, int posY) const
{
    return field[posY][posX] != nullptr;
}

int TetrisField::get_height () const
{
    return cellsVer;
}

void TetrisField::add_block (int posX, int posY, Block *block)
{
    field[posY][posX] = block;
}

int TetrisField::clear_lines ()
{
    int shift = 0;
    for (int row = cellsVer - 1, col; row - shift >= 0; --row)
    {
        field[row] = field[row - shift];
        for (col = 0; col < cellsHor; ++col)
        {
            if (field[row][col] == nullptr)
            {
                break;
            }
        }
        if (col == cellsHor)
        {
            for (int col = 0; col < cellsHor; ++col)
            {
                delete field[row][col];
            }
            ++shift;
            ++row;
        }
    }
    for (int row = 0; row < shift; ++row)
    {
        field[row] = std::vector<Block *>(cellsHor, nullptr);
    }

    return shift;
}
