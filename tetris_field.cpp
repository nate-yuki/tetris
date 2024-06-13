#include "tetris_field.hpp"


Block::Block (Texture *texture, const SDL_Rect *clip)
    : texture(texture)
    , clip(clip)
{}

void Block::render (int x, int y, int s)
{
    texture->render({x, y, s, s}, clip);
}
