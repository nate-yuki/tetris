/**
 * @file  tetris_layout.cpp
 * @brief Implementation of TetrisLayout class.
 */

#include "tetris_layout.hpp"
#include "game.hpp"
#include "key_layout.hpp"
#include "constants.hpp"
#include "logger.hpp"


void TetrisLayout::init (
    int cellsHor, int cellsVer,
    KeyLayout *tetrisKeyLayout, KeyLayout *tetriminoKeyLayout,
    Timer *tetriminoTimer, Timer *clearLineTimer, Timer *gameOverTimer,
    Timer *msgTextTimer,
    Texture *bgTexture, Texture *blockTextureSheet,
    Texture *fieldBgTexture, Texture *fieldFrameTexture, Texture *fieldClearTexture,
    Texture *fieldClearParticleTextureSheet,
    Text *linesClearedText, Text *linesClearedPromptText,
    Text *scoreText, Text *scorePromptText,
    Text *highScoreText, Text *highScorePromptText,
    Text *msgText, Text *comboText
)
{
    field.init(
        cellsHor, cellsVer, fieldBgTexture, fieldFrameTexture, fieldClearTexture,
        fieldClearParticleTextureSheet
    );

    msg.init(msgText, msgTextTimer);

    tetrimino.init(&field, blockTextureSheet, tetriminoKeyLayout);

    for (int i = 0; i < TETRIMINO_QUEUE_LEN; ++i)
    {
        tetriminoQueue.push_back(TetriminoConfig());
    }
    tetriminoSwap = nullptr;
    swapped = 0;

    tetriminoFallDelay = TETRIMINO_INITIAL_FALL_DELAY;

    gameOver = false;

    linesCleared = score = combo = 0;

    keyLayout = tetrisKeyLayout;
    this->bgTexture = bgTexture;
    this->blockTextureSheet = blockTextureSheet;
    this->linesClearedText = linesClearedText;
    this->linesClearedPromptText = linesClearedPromptText;
    this->scoreText = scoreText;
    this->scorePromptText = scorePromptText;
    this->highScoreText = highScoreText;
    this->highScorePromptText = highScorePromptText;
    this->msgText = msgText;
    this->comboText = comboText;
    this->tetriminoTimer = tetriminoTimer;
    this->clearLineTimer = clearLineTimer;
    this->gameOverTimer = gameOverTimer;
}

void TetrisLayout::free ()
{
    field.free();
    tetrimino.free();
    tetriminoQueue.resize(0);
}

void TetrisLayout::handle_event (Game &game, const SDL_Event &e)
{
    tetrimino.handle_event(game, e);

    if (!game.is_paused())
    {
        keyLayout->handle_event(game, e);
        if (keyLayout->get_type() == SDL_KEYDOWN && keyLayout->get_repeat() == 0)
        {
            switch (keyLayout->get_map())
            {
            case SWAP:
                swap();
                break;
            }
        }
    }
}

void TetrisLayout::do_logic ()
{
    tetrimino.move(tetriminoTimer->get_elapsed());
    if (tetrimino.fall(tetriminoTimer->get_elapsed()))
    {
        int currLinesCleared = field.clear_lines();
        if (currLinesCleared)
        {
            clearLineTimer->start();
        }
        manage_score(currLinesCleared);
        
        if (--tetriminoFallDelay < TETRIMINO_MIN_FALL_DELAY)
        {
            tetriminoFallDelay = TETRIMINO_MIN_FALL_DELAY;
        }

        spawn_tetrimino();
    }
    tetriminoTimer->start();
}

void TetrisLayout::render (int x, int y, int w, int h)
{
    bgTexture->render({x, y, w, h});

    int fieldW = w / 3, fieldH = 3 * h / 4;
    int fieldX = x + (w - fieldW) / 2, fieldY = y + (h - fieldH) / 2;
    int blockSize = min(fieldW / field.get_width(), fieldH / field.get_height());

    // Render the tetrimino queue
    for (int i = 0; i < tetriminoQueue.size(); ++i)
    {
        Tetrimino::render_config(
            tetriminoQueue[i],
            fieldX + fieldW + blockSize,
            fieldY + i * (MAX_SCHEME_LEN + 1) * blockSize / 2,
            blockSize / 2,
            blockTextureSheet
        );
    }
    // Render the swap tetrimino
    if (tetriminoSwap != nullptr)
    {
        Tetrimino::render_config(
            *tetriminoSwap,
            fieldX - (MAX_SCHEME_LEN + 1) * blockSize,
            fieldY,
            blockSize,
            blockTextureSheet
        );
    }

    int promptW = blockSize * 9, promptH = blockSize;
    int promptSpace = promptH / 2;
    int promptX = fieldX - blockSize - promptW;

    // Render cleared lines info under the swap tetrimino
    int linesY = fieldY + (MAX_SCHEME_LEN + 1) * blockSize;
    linesClearedPromptText->render(
        promptX,
        linesY,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    linesClearedText->render(
        promptX,
        linesY + promptSpace + promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_RIGHT
    );

    // Render score info adjacent to the field bottom
    int scoreBottomY = fieldY + fieldH;
    scorePromptText->render(
        promptX,
        scoreBottomY - 3 * promptH - 3 * promptSpace - promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    scoreText->render(
        promptX,
        scoreBottomY - 2 * promptH - promptSpace - 2 * promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_RIGHT
    );
    highScorePromptText->render(
        promptX,
        scoreBottomY - promptH - promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    highScoreText->render(
        promptX,
        scoreBottomY - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_RIGHT
    );

    // Render combo info above the field
    int comboY = y + h / 32;
    int comboH = blockSize;
    comboText->render(
        fieldX,
        comboY,
        fieldW,
        fieldY - comboH - comboY
    );

    // Render the message bellow the field
    int msgW = 7 * w / 8;
    int msgH = 31 * h / 32 - (fieldY - y) - fieldH - blockSize;
    int msgX = x + (w - msgW) / 2, msgY = fieldY + fieldH + blockSize;
    msg.render(
        msgX,
        msgY,
        msgW,
        msgH
    );

    field.render(
        fieldX, fieldY, fieldW, fieldH,
        tetrimino, clearLineTimer->get_elapsed() >= CLEAR_LINE_RENDER_TIME
    );
}

bool TetrisLayout::game_over () const
{
    return gameOver;
}

int TetrisLayout::get_score () const
{
    return score;
}

void TetrisLayout::spawn_tetrimino ()
{
    bool tetriminoFit = tetrimino.spawn(
        (field.get_width() - MAX_SCHEME_LEN) / 2, 0, tetriminoFallDelay,
        tetriminoQueue.front()
    );
    if (!tetriminoFit)
    {
        // Tetrimino could not be spawned because of obstructing field blocks
        gameOverTimer->start();
        gameOver = true;
    }
    tetriminoQueue.pop_front();
    // If swapped > 0, decreases it
    // If then swapped == 1, does nothing, if otherwise swapped == 0, fills the queue
    if (!swapped || !--swapped)
    {
        tetriminoQueue.push_back(TetriminoConfig());
    }
}

void TetrisLayout::swap()
{
    if (!swapped)
    {
        swapped = 2;
        if (tetriminoSwap != nullptr)
        {
            // If there were swaps, put the current tetrimino to the queue front
            tetriminoQueue.push_front(*tetriminoSwap);
            delete tetriminoSwap;
        }
        else
        {
            // If there were no swaps, add to the queue so it doesn't get shortened
            tetriminoQueue.push_back(TetriminoConfig());
        }
        // Move the current tetrimino to the swap buffer
        tetriminoSwap = new TetriminoConfig(tetrimino.get_config());
        tetrimino.free();

        // Spawn a new tetrimino
        spawn_tetrimino();
    }
}

void TetrisLayout::manage_score (int currLinesCleared)
{
    switch (currLinesCleared)
    {
    case 0:
        if (combo)
        {
            comboText->set_text("Combo: 0");
            combo = 0;
        }
        break;
    case 1:
        msg.set_text("Line clear", TETRIS_MSG_TIME, &WHITE);
        break;
    case 2:
        msg.set_text("2 lines cleared!", TETRIS_MSG_TIME, &CYAN);
        break;
    case 3:
        msg.set_text("3 lines cleared!", TETRIS_MSG_TIME, &YELLOW);
        break;
    case 4:
        msg.set_text("TETRIS!", TETRIS_MSG_TIME, &RED);
        score += SCORE_TETRIS;
        break;
    }
    if (currLinesCleared)
    {
        score += currLinesCleared * MULT_LINE + combo * MULT_COMBO;
        scoreText->set_text(get_padded(std::to_string(score), 9, '0'));

        comboText->set_text("Combo: " + std::to_string(++combo));
        linesClearedText->set_text(
            get_padded(std::to_string(linesCleared += currLinesCleared), 4, '0')
        );
    }
}
