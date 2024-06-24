#include "tetris_layout.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "logger.hpp"


void TetrisLayout::init (
    int cellsHor, int cellsVer,
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

    Tetrimino::load_schemes("schemes.txt");
    Tetrimino::init_clips();
    tetrimino.init(&field, blockTextureSheet);

    for (int i = 0; i < TETRIMINO_QUEUE_LEN; ++i)
    {
        tetriminoQueue.push_back(TetriminoConfig());
    }
    tetriminoSwap = nullptr;
    swapped = 0;

    tetriminoFallDelay = TETRIMINO_INITIAL_FALL_DELAY;

    gameOver = false;

    linesCleared = 0;

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
}

void TetrisLayout::handle_event (Game &game, const SDL_Event &e)
{
    if (!game.is_paused() && !gameOver)
    {
        tetrimino.handle_event(game, e);
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_SPACE:
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
        spawn_tetrimino();
        if (--tetriminoFallDelay < TETRIMINO_MIN_FALL_DELAY)
        {
            tetriminoFallDelay = TETRIMINO_MIN_FALL_DELAY;
        }
    }
    tetriminoTimer->start();
}

void TetrisLayout::render (int x, int y, int w, int h)
{
    bgTexture->render(
        x + (w - bgTexture->get_width()) / 2, y + (h - bgTexture->get_height()) / 2
    );

    int blockSize = min(w / 3 / TETRIS_FIELD_WIDTH, 3 * h / 4 / TETRIS_FIELD_HEIGHT);
    for (int i = 0; i < tetriminoQueue.size(); ++i)
    {
        Tetrimino::render_config(
            tetriminoQueue[i],
            x + 2 * w / 3 + blockSize,
            y + h / 8 + i * (MAX_SCHEME_LEN + 1) * blockSize / 2,
            blockSize / 2,
            blockTextureSheet
        );
    }
    if (tetriminoSwap != nullptr)
    {
        Tetrimino::render_config(
            *tetriminoSwap,
            x + w / 3 - (MAX_SCHEME_LEN + 1) * blockSize,
            y + h / 8,
            blockSize,
            blockTextureSheet
        );
    }

    linesClearedPromptText->render(
        x + w / 3 - 10 * blockSize,
        y + h / 8 + (MAX_SCHEME_LEN + 1) * blockSize,
        blockSize * 9,
        blockSize,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    linesClearedText->render(
        x + w / 3 - 10 * blockSize,
        y + h / 8 + (MAX_SCHEME_LEN + 1) * blockSize + 3 * blockSize / 2,
        blockSize * 9,
        blockSize,
        Text::TextAlign::TEXT_CENTER_RIGHT
    );

    scorePromptText->render(
        x + w / 3 - 10 * blockSize,
        y + 7 * h / 8 - 6 * blockSize,
        blockSize * 9,
        blockSize,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    scoreText->render(
        x + w / 3 - 10 * blockSize,
        y + 7 * h / 8 - 9 * blockSize / 2,
        blockSize * 9,
        blockSize,
        Text::TextAlign::TEXT_CENTER_RIGHT
    );
    highScorePromptText->render(
        x + w / 3 - 10 * blockSize,
        y + 7 * h / 8 - 5 * blockSize / 2,
        blockSize * 9,
        blockSize,
        Text::TextAlign::TEXT_CENTER_LEFT
    );
    highScoreText->render(
        x + w / 3 - 10 * blockSize,
        y + 7 * h / 8 - blockSize,
        blockSize * 9,
        blockSize,
        Text::TextAlign::TEXT_CENTER_RIGHT
    );

    comboText->render(
        x + w / 3,
        h / 32,
        w / 3,
        y + h / 8 - blockSize - h / 32
    );

    msg.render(
        x + w / 16,
        y + 7 * h / 8 + blockSize,
        7 * w / 8,
        31 * h / 32 - (y + 7 * h / 8 + blockSize)
    );

    clearLineTimer->get_elapsed();
    field.render(
        x + w / 3, y + h / 8, w / 3, 3 * h / 4,
        tetrimino, clearLineTimer->get_elapsed() >= CLEAR_LINE_RENDER_TIME
    );
}

bool TetrisLayout::game_over () const
{
    return gameOver;
}

void TetrisLayout::spawn_tetrimino ()
{
    bool tetriminoFit = tetrimino.spawn(
        (TETRIS_FIELD_WIDTH - MAX_SCHEME_LEN) / 2, 0, tetriminoFallDelay,
        tetriminoQueue.front()
    );
    if (!tetriminoFit)
    {
        gameOverTimer->start();
        gameOver = true;
    }
    tetriminoQueue.pop_front();
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
            tetriminoQueue.push_front(*tetriminoSwap);
            delete tetriminoSwap;
        }
        else
        {
            tetriminoQueue.push_back(TetriminoConfig());
        }
        tetriminoSwap = new TetriminoConfig(tetrimino.get_config());
        tetrimino.free();
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
