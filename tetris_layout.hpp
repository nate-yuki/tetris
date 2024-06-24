#ifndef TETRIS_LAYOUT_HPP
#define TETRIS_LAYOUT_HPP


#include "tetris_field.hpp"
#include "tetrimino.hpp"
#include "texture.hpp"
#include "text.hpp"
#include "timer.hpp"
#include "timed_media.hpp"

#include <SDL2/SDL.h>
#include <deque>


class TetrisLayout
{
public:
    void init(
        int cellsHor, int cellsVer,
        Timer *tetriminoTimer, Timer *clearLineTimer, Timer *gameOverTimer,
        Timer *msgTextTimer,
        Texture *bgTexture, Texture *blockTextureSheet, Texture *fieldBgTexture,
        Texture *fieldFrameTexture, Texture *fieldClearTexture,
        Texture *fieldClearParticleTextureSheet,
        Text *linesClearedText, Text *linesClearedPromptText,
        Text *scoreText, Text *scorePromptText,
        Text *highScoreText, Text *highScorePromptText,
        Text *msgText, Text *comboText
    );
    void free();

    void handle_event(Game &game, const SDL_Event &e);
    void do_logic();
    void render(int x, int y, int w, int h);

    bool game_over() const;

private:
    static constexpr int MULT_LINE = 1000;
    static constexpr int MULT_COMBO = 1500;
    static constexpr int SCORE_TETRIS = 1000;

    void spawn_tetrimino();
    void swap();
    void manage_score(int currLinesCleared);

    Texture *bgTexture, *blockTextureSheet;
    Text *linesClearedText, *linesClearedPromptText;
    Text *scoreText, *scorePromptText, *highScoreText, *highScorePromptText;
    Text *msgText, *comboText;
    TimedText msg;
    Timer *tetriminoTimer, *clearLineTimer, *gameOverTimer;

    TetrisField field;
    Tetrimino tetrimino;

    std::deque<TetriminoConfig> tetriminoQueue;
    TetriminoConfig *tetriminoSwap;
    int swapped;
    int tetriminoFallDelay;
    int gameOver;

    int linesCleared;
    int score, combo;
};


#endif
