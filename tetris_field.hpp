#ifndef TETRIS_FIELD_HPP
#define TETRIS_FIELD_HPP


#include "texture.hpp"
#include "tetrimino.hpp"

#include <SDL2/SDL.h>
#include <list>
#include <vector>


class Tetrimino;

/// A tetrimino block class.
class Block
{
public:
    /**
     * @brief Initialize class members.
     * @param blockTextureSheet A block textures sheet.
     * @param clip A clip from `blockTextureSheet` to use as the block's texture.
     */
    Block(Texture *blockTextureSheet, const SDL_Rect *clip);

    /**
     * @brief Draw the block with given location and dimensions.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param size Block size.
     */
    void render(int x, int y, int size);

private:
    Texture *texture;
    const SDL_Rect *clip;
};

class TetrisField
{
public:
    void init(int cellsHor, int cellsVer, Texture *bgTexture, Texture *frameTexture);
    void free();
    void render(int x, int y, int w, int h, Tetrimino &tetrimino);
    bool has_block(int posX, int posY) const;
    int get_height() const;
    void add_block(int posX, int posY, Block *block);
    int clear_lines();

private:
    Texture *bgTexture, *frameTexture;
    std::vector<std::vector<Block *>> field;
    int cellsHor, cellsVer;
};


#endif
