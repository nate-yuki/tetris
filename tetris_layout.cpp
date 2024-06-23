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
    Text *linesClearedText, Text *msgText
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
    newLinesCleared = false;

    this->bgTexture = bgTexture;
    this->blockTextureSheet = blockTextureSheet;
    this->linesClearedText = linesClearedText;
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
        if (int currLinesCleared = field.clear_lines())
        {
            clearLineTimer->start();
            linesCleared += currLinesCleared;
            newLinesCleared = true;

            msg.set_text(
                std::to_string(currLinesCleared) + " line(s) cleared!",
                TETRIS_MSG_TIME
            );
        }
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

    if (newLinesCleared)
    {
        linesClearedText->set_text(std::to_string(linesCleared));
        newLinesCleared = false;
    }
    linesClearedText->render(
        x + w / 3 - (MAX_SCHEME_LEN + 1) * blockSize,
        y + h / 8 + (MAX_SCHEME_LEN + 1) * blockSize,
        blockSize * 4,
        blockSize
    );

    clearLineTimer->get_elapsed();
    field.render(
        x + w / 3, y + h / 8, w / 3, 3 * h / 4,
        tetrimino, clearLineTimer->get_elapsed() >= CLEAR_LINE_RENDER_TIME
    );

    msg.render(
        x + w / 16, y + 7 * h / 8 + blockSize, 7 * w / 8,
        31 * h / 32 - (y + 7 * h / 8 + blockSize)
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
