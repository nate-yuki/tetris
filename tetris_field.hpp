#ifndef TETRIS_FIELD_HPP
#define TETRIS_FIELD_HPP


#include "texture.hpp"

#include <SDL2/SDL.h>
#include <list>
#include <vector>


/// A tetrimino block class.
class Block
{
public:
    /**
     * @brief Initialize class members.
     * @param texture The block texture.
     */
    void init(Texture *texture, const SDL_Rect &clip);

    /**
     * @brief Draw the block with given location and dimensions.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param s Block size.
     */
    void render(int x, int y, int s);

private:
    Texture *texture;
    SDL_Rect clip;
};

class TetrisField
{
public:

private:
    std::list<std::vector<Block *>> field;
};


#endif
