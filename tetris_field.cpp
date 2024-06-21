#include "tetris_field.hpp"
#include "logger.hpp"


Block::Block (Texture *blockTextureSheet, const SDL_Rect *clip)
    : blockTextureSheet(blockTextureSheet)
    , clip(clip)
{}

void Block::render (int x, int y, int size)
{
    blockTextureSheet->render({x, y, size, size}, clip);
}


void TetrisField::init (
    int cellsHor, int cellsVer,
    Texture *bgTexture, Texture *frameTexture, Texture *clearTexture,
    Texture *particleTextureSheet
)
{
    log("Initializing TetrisField", __FILE__, __LINE__);
    
    field = std::vector<std::vector<Block *>>(
        cellsVer, std::vector<Block *>(cellsHor, nullptr)
    );
    clearedLines = {-1};

    this->cellsHor = cellsHor;
    this->cellsVer = cellsVer;
    this->bgTexture = bgTexture;
    this->frameTexture = frameTexture;
    this->clearTexture = clearTexture;
    this->particleTextureSheet = particleTextureSheet;
}

void TetrisField::free()
{
    log("Freeing TetrisField", __FILE__, __LINE__);
    
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

void TetrisField::render (
    int x, int y, int w, int h, Tetrimino &tetrimino, bool stopClearLineRender
)
{
    frameTexture->render({x, y, w, h});

    int size = min(w / cellsHor, h / cellsVer);
    int fieldX = x + (w - size * cellsHor) / 2;
    int fieldY = y + (h - size * cellsVer) / 2;

    int shift = 0;
    for (int row = cellsVer - 1; row - shift >= 0; --row)
    {
        if (clearedLines[shift] == row - shift)
        {
            clearTexture->render(
                {fieldX, fieldY + (row - shift) * size, size * cellsHor, size}
            );
            ++shift;
            ++row;

            if (clearLineParticlers.size() < shift)
            {
                clearLineParticlers.push_back(new ParticleEmmiter(
                    CLEAR_LINE_PARTICLES_MAX, CLEAR_LINE_PARTICLE_LIFESPAN,
                    CLEAR_LINE_PARTICLE_SHIFT_MAX, particleTextureSheet
                ));
            }
        }
        else
        {
            for (int col = 0; col < cellsHor; ++col)
            {
                if (field[row][col] != nullptr)
                {
                    field[row][col]->render(
                        fieldX + col * size, fieldY + (row - shift) * size, size
                    );
                }
                else
                {
                    bgTexture->render(
                        {
                            fieldX + col * size, fieldY + (row - shift) * size,
                            size, size
                        }
                    );
                }
            }
        }
    }

    tetrimino.render(fieldX, fieldY, size);

    if (stopClearLineRender)
    {
        clearedLines = {-1};
        for (ParticleEmmiter *&particler : clearLineParticlers)
        {
            delete particler;
        }
        clearLineParticlers.resize(0);
    }
    else
    {
        for (int i = 0; i < clearLineParticlers.size(); ++i)
        {
            clearLineParticlers[i]->render(
                fieldX, fieldY + clearedLines[i] * size, size * cellsHor, size, 
                size / 4
            );
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
    clearedLines.resize(0);
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

            clearedLines.push_back(row - shift);
        }
    }
    for (int row = 0; row < shift; ++row)
    {
        field[row] = std::vector<Block *>(cellsHor, nullptr);
    }
    clearedLines.push_back(-1);

    return shift;
}
