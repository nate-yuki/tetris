/**
 * @file  tetris_layout.hpp
 * @brief Include file for TetrisLayout class.
 */

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


class KeyLayout;

/// A complete tetris setup class with logic, event handling and scoring.
class TetrisLayout
{
public:
    /// Tetris commands.
    enum Commands{
        SWAP, // Swap the tetrimino with the buffered one.
    };

    /// Tetris layout settings.
    enum Layout{
        FULL, // The full layout.
        REDUCED, // No high score.
        MINIMAL, // No high score, no lines cleared, no messages.
    };

    /**
     * @brief Initialize class members.
     * @param cellsHor Amount of cells in each row.
     * @param cellsVer Amount of cells in each column.
     * @param tetrisKeyLayout Key layout to use for the tetris layout.
     * @param tetriminoKeyLayout Key layout to use for the tetrimino.
     * @param tetriminoTimer Timer to use for tetrimino movement.
     * @param clearLineTimer Timer to use to time clear line rendering.
     * @param gameOverTimer Timer to start when the game is over.
     * @param msgTextTimer Timer to use for timed text used for messages.
     * @param bgTexture Texture to use as the whole layout background.
     * @param blockTextureSheet The block texture sheet: last entry is the ghost
     *     tetrimino texture and all other are for different block textures.
     * @param fieldBgTexture Texture to use for tetris grid empty cells.
     * @param fieldFrameTexture Texture to render behind the field.
     * @param fieldClearTexture Texture to use for field cleared rows.
     * @param fieldClearParticleTextureSheet Particle texture sheet to use for cleared
     *     row particles.
     * @param linesClearedText Text to use for cleared lines amount display.
     * @param linesClearedPromptText Text to use for cleared lines prompt display.
     * @param scoreText Text to use for score display.
     * @param scorePromptText Text to use for score prompt display.
     * @param highScoreText Text to use for high score display.
     * @param highScorePromptText Text to use for high score prompt display.
     * @param msgText Text to use for messages.
     * @param comboText Text to use for current combo display.
     * @param layout Controls how much is displayed; default is `FULL`.
     */
    void init(
        int cellsHor, int cellsVer,
        KeyLayout *tetrisKeyLayout, KeyLayout *tetriminoKeyLayout,
        Timer *tetriminoTimer, Timer *clearLineTimer, Timer *gameOverTimer,
        Timer *msgTextTimer,
        Texture *bgTexture, Texture *blockTextureSheet, Texture *fieldBgTexture,
        Texture *fieldFrameTexture, Texture *fieldClearTexture,
        Texture *fieldClearParticleTextureSheet,
        Text *linesClearedText, Text *linesClearedPromptText,
        Text *scoreText, Text *scorePromptText,
        Text *highScoreText, Text *highScorePromptText,
        Text *msgText, Text *comboText,
        Layout layout=FULL
    );

    /// Free the class members.
    void free();

    /**
     * @brief If the game is not over, handle events.
     * @details
     * Handles tetrimino events, and, if the game is not paused, swaps on `SWAP` key
     * press.
     */
    void handle_event(Game &game, const SDL_Event &e);

    /**
     * @brief Do tetris logic.
     * @details
     * Moves the tetrimino. If the tetrimino fell, checks for and clears filled lines,
     * manages the score, increases tetrimino falling speed and spawns a new
     * tetrimino.
     */
    void do_logic();

    /**
     * @brief Render the tetrimino layout UI.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w The width.
     * @param h The Height.
     */
    void render(int x, int y, int w, int h);

    /// `true` if a new tetrimino could not be spawned.
    bool game_over() const;

    /// Get the current score.
    int get_score() const;

private:
    static constexpr int MULT_LINE = 1000; /// Score per cleared line.

    /// Score per combo accumulated before clearing a line.
    static constexpr int MULT_COMBO = 1500;

    static constexpr int SCORE_TETRIS = 1000; /// Additional score for a 4-line clear.

    /// Render the full layout.
    void render_full(int x, int y, int w, int h);

    /// Render the reduced layout.
    void render_reduced(int x, int y, int w, int h);

    /// Render the minimal layout.
    void render_minimal(int x, int y, int w, int h);

    /**
     * @brief Spawn a new tetrimino with the config from the front of the queue.
     * @details
     * If the tetrimino could not be spawned, sets `gameOver` and starts
     * `gameOverTimer`. If this is not the first spawn since the last swap, adds a new
     * config to the back of the tetrimino queue.
     */
    void spawn_tetrimino();

    /// If the previous swapped tetrimino was released, or there were no swaps, swap.
    void swap();

    /// Add score, set combo, and set message, score, and combo texts.
    void manage_score(int currLinesCleared);

    KeyLayout *keyLayout;
    Texture *bgTexture, *blockTextureSheet;
    Text *linesClearedText, *linesClearedPromptText;
    Text *scoreText, *scorePromptText, *highScoreText, *highScorePromptText;
    Text *msgText, *comboText;
    TimedText msg;
    Timer *tetriminoTimer, *clearLineTimer, *gameOverTimer;

    TetrisField field;
    Tetrimino tetrimino;

    Layout layout;
    std::deque<TetriminoConfig> tetriminoQueue;
    TetriminoConfig *tetriminoSwap;

    /**
     * @brief If true, swap when doing logic.
     * @note
     * Swapping in the event loop might cause the following bug: pressing swap key and
     * then quickly pressing and holding a movement key may cause the new tetrimino to
     * increase its velocity after getting key states on spawn, and then increase the
     * same velocity during the key press event handling, leading to the tetrimino
     * moving on its own.
     */
    bool trySwap;

    int swapped; // Amount of tetrimino spawns since the last swap.
    int tetriminoFallDelay;
    int gameOver;

    int linesCleared; // Total lines cleared.
    int score, combo;
};


#endif
