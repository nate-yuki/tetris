#ifndef TETRIS_LAYOUT_HPP
#define TETRIS_LAYOUT_HPP


#include "tetris_field.hpp"
#include "tetrimino.hpp"
#include "texture.hpp"
#include "timer.hpp"

#include <SDL2/SDL.h>
#include <deque>


class TetrisLayout
{
public:
    void init(
        int cellsHor, int cellsVer,
        Timer *tetriminoTimer, Timer *clearLineTimer, Timer *gameOverTimer,
        Texture *bgTexture, Texture *blockTextureSheet, Texture *fieldBgTexture,
        Texture *fieldFrameTexture, Texture *fieldClearTexture,
        Texture *fieldClearParticleTextureSheet
    );
    void free();

    void handle_event(Game &game, const SDL_Event &e);
    void do_logic();
    void render(int x, int y, int w, int h);

    bool game_over() const;

private:
    Texture *bgTexture, *blockTextureSheet;
    Timer *tetriminoTimer, *clearLineTimer, *gameOverTimer;

    TetrisField field;
    Tetrimino tetrimino;

    std::deque<TetriminoConfig> tetriminoQueue;
    TetriminoConfig *tetriminoSwap;
    int tetriminoFallDelay;
    int gameOver;
};


#endif
