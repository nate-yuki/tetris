/**
 * @file  tetris_layout.cpp
 * @brief Implementation of TetrisLayout class.
 */

#include "tetris_layout.hpp"
#include "game.hpp"
#include "audio.hpp"
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
    Text *msgText, Text *comboText,
    Layout layout
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
    trySwap = false;
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
    this->comboText = comboText;
    this->tetriminoTimer = tetriminoTimer;
    this->clearLineTimer = clearLineTimer;
    this->gameOverTimer = gameOverTimer;
    this->layout = layout;
}

void TetrisLayout::free ()
{
    field.free();
    tetrimino.free();
    tetriminoQueue.resize(0);
}

void TetrisLayout::handle_event (Game &game, const SDL_Event &e)
{
    if (gameOver)
    {
        return;
    }
    
    // Only handle current tetrimino event if it is not being swapped
    if (!trySwap)
    {
        tetrimino.handle_event(game, e);
    }

    if (!game.is_paused())
    {
        keyLayout->handle_event(game, e);
        if (keyLayout->get_type() == KeyLayout::DOWN && keyLayout->get_repeat() == 0)
        {
            switch (keyLayout->get_command())
            {
            case SWAP:
                // Delegate swapping to logic
                trySwap = true;
                break;
            }
        }
    }
}

void TetrisLayout::do_logic ()
{
    if (trySwap)
    {
        swap();
        trySwap = false;
    }
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

    switch (layout)
    {
    case FULL:
        render_full(x, y, w, h);
        break;
    case REDUCED:
        render_reduced(x, y, w, h);
        break;
    case MINIMAL:
        render_minimal(x, y, w, h);
        break;
    }
}

bool TetrisLayout::game_over () const
{
    return gameOver;
}

int TetrisLayout::get_score () const
{
    return score;
}

void TetrisLayout::render_full (int x, int y, int w, int h)
{
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
    if (highScorePromptText != nullptr && highScoreText != nullptr)
    {
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
    }

    // Render combo info above the field
    int comboSpace = h / 32;
    int comboH = min(fieldY - y - 2 * comboSpace, blockSize);
    comboText->render(
        fieldX,
        fieldY - comboSpace - comboH,
        fieldW,
        comboH
    );

    // Render the message bellow the field
    int msgW = 7 * w / 8;
    int msgSpace = h / 32;
    int msgH = h - 2 * msgSpace - (fieldY - y) - fieldH;
    int msgX = x + (w - msgW) / 2, msgY = fieldY + fieldH + msgSpace;
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

void TetrisLayout::render_reduced (int x, int y, int w, int h)
{
    int fieldW = w / 2, fieldH = 3 * h / 4;
    int fieldX = x + w / 12, fieldY = y + (h - fieldH) / 2;
    int blockSize = min(fieldW / field.get_width(), fieldH / field.get_height());

    // Render the swap tetrimino
    if (tetriminoSwap != nullptr)
    {
        Tetrimino::render_config(
            *tetriminoSwap,
            fieldX + fieldW + blockSize,
            fieldY,
            blockSize,
            blockTextureSheet
        );
    }
    // Render the tetrimino queue
    int queueBegX = fieldX + fieldW + 3 * blockSize / 4;
    int queueBegY = fieldY + (MAX_SCHEME_LEN + 1) * blockSize;
    int firstColRows = tetriminoQueue.size() / 2 + tetriminoQueue.size() % 2;
    for (int i = 0; i < tetriminoQueue.size(); ++i)
    {
        int col = i >= firstColRows;
        int row = i - col * firstColRows;
        Tetrimino::render_config(
            tetriminoQueue[i],
            queueBegX + col * (MAX_SCHEME_LEN + 1) * blockSize / 2,
            queueBegY + row * (MAX_SCHEME_LEN + 1) * blockSize / 2,
            blockSize / 2,
            blockTextureSheet
        );
    }

    // Render score and lines cleared info adjacent to the field bottom
    int promptW = blockSize * 9, promptH = blockSize;
    int promptSpace = promptH / 2;
    int promptX = fieldX + fieldW + blockSize;
    int scoreBottomY = fieldY + fieldH;
    linesClearedPromptText->render(
        promptX,
        scoreBottomY - 3 * promptH - 3 * promptSpace - promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    linesClearedText->render(
        promptX,
        scoreBottomY - 2 * promptH - promptSpace - 2 * promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    scorePromptText->render(
        promptX,
        scoreBottomY - promptH - promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    scoreText->render(
        promptX,
        scoreBottomY - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );

    // Render combo info above the field
    int comboSpace = h / 32;
    int comboH = min(fieldY - y - 2 * comboSpace, blockSize);
    comboText->render(
        fieldX,
        fieldY - comboSpace - comboH,
        fieldW,
        comboH
    );

    // Render the message bellow the field
    int msgW = 7 * w / 8;
    int msgSpace = h / 32;
    int msgH = h - 2 * msgSpace - (fieldY - y) - fieldH;
    int msgX = x + (w - msgW) / 2, msgY = fieldY + fieldH + msgSpace;
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

void TetrisLayout::render_minimal (int x, int y, int w, int h)
{
    int paddingHor = w / 32;
    int fieldW = 2 * w / 3, fieldH = 13 * h / 16;
    int fieldX = x + paddingHor, fieldY = y + h - fieldH - h / 32;
    int blockSize = min(fieldW / field.get_width(), fieldH / field.get_height());

    // Render the swap tetrimino
    if (tetriminoSwap != nullptr)
    {
        Tetrimino::render_config(
            *tetriminoSwap,
            fieldX + fieldW + blockSize / 2,
            fieldY,
            blockSize,
            blockTextureSheet
        );
    }
    // Render the tetrimino queue adjacent to the field bottom
    int queueBegX = fieldX + fieldW + blockSize / 2 + blockSize / 2;
    int queueEndY = fieldY + fieldH;
    for (int i = 0; i < tetriminoQueue.size(); ++i)
    {
        Tetrimino::render_config(
            tetriminoQueue[i],
            queueBegX,
            queueEndY - (tetriminoQueue.size() - i) * MAX_SCHEME_LEN * blockSize / 2
                - (tetriminoQueue.size() - i - 1) * blockSize / 2,
            blockSize / 2,
            blockTextureSheet
        );
    }

    // Render combo info above the field in the left
    int promptSpace = h / 64;
    int promptW = w - fieldW - blockSize - 2 * paddingHor;
    int promptH = fieldY - y - 2 * promptSpace;
    comboText->render(
        fieldX,
        fieldY - promptSpace - promptH,
        promptW,
        promptH,
        Text::TEXT_CENTER_LEFT
    );
    // Render score info above the field in the right
    scorePromptText->render(
        fieldX + fieldW - promptW,
        fieldY - promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_RIGHT
    );
    scoreText->render(
        fieldX + fieldW + blockSize,
        fieldY - promptSpace - promptH,
        promptW,
        promptH,
        Text::TextAlign::TEXT_CENTER_LEFT
    );

    field.render(
        fieldX, fieldY, fieldW, fieldH,
        tetrimino, clearLineTimer->get_elapsed() >= CLEAR_LINE_RENDER_TIME
    );
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

        Audio::play_sound(Audio::TETRIS_GAME_OVER);
    }
    tetriminoQueue.pop_front();
    // If swapped > 0, decreases it
    // If then swapped == 1, does nothing, if otherwise swapped == 0, fills the queue
    if (!swapped || !--swapped)
    {
        tetriminoQueue.push_back(TetriminoConfig());
    }
}

void TetrisLayout::swap ()
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

        Audio::play_sound(Audio::TETRIS_SWAP);
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
        
        Audio::play_sound(Audio::TETRIS_SINGLE);
        break;
    case 2:
        msg.set_text("2 lines cleared!", TETRIS_MSG_TIME, &CYAN);
        
        Audio::play_sound(Audio::TETRIS_DOUBLE);
        break;
    case 3:
        msg.set_text("3 lines cleared!", TETRIS_MSG_TIME, &YELLOW);
        
        Audio::play_sound(Audio::TETRIS_TRIPLE);
        break;
    case 4:
        msg.set_text("TETRIS!", TETRIS_MSG_TIME, &RED);
        score += SCORE_TETRIS;
        
        Audio::play_sound(Audio::TETRIS_TETRIS);
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
