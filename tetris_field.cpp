#include "tetris_field.hpp"


void Block::init (Texture *texture, const SDL_Rect &clip)
{
    this->texture = texture;
    this->clip = clip;
}

void Block::render (int x, int y, int s)
{
    texture->render({x, y, s, s}, &clip);
}
