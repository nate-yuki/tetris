#include "tetris_layout.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "logger.hpp"


void TetrisLayout::init (
    int cellsHor, int cellsVer,
    Timer *tetriminoTimer, Timer *clearLineTimer, Timer *gameOverTimer,
    Texture *bgTexture, Texture *blockTextureSheet,
    Texture *fieldBgTexture, Texture *fieldFrameTexture, Texture *fieldClearTexture,
    Texture *fieldClearParticleTextureSheet
)
{
    field.init(
        cellsHor, cellsVer, fieldBgTexture, fieldFrameTexture, fieldClearTexture,
        fieldClearParticleTextureSheet
    );

    Tetrimino::load_schemes("schemes.txt");
    Tetrimino::init_clips();
    tetrimino.init(&field, blockTextureSheet);

    for (int i = 0; i < TETRIMINO_QUEUE_LEN; ++i)
    {
        tetriminoQueue.push_back(TetriminoConfig());
    }
    tetriminoSwap = nullptr;

    tetriminoFallDelay = TETRIMINO_INITIAL_FALL_DELAY;

    gameOver = false;

    this->bgTexture = bgTexture;
    this->blockTextureSheet = blockTextureSheet;
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
    }
}

void TetrisLayout::do_logic ()
{
    tetrimino.move(tetriminoTimer->get_elapsed());
    if (tetrimino.fall(tetriminoTimer->get_elapsed()))
    {
        if (field.clear_lines())
        {
            clearLineTimer->start();
        }

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
        tetriminoQueue.push_back(TetriminoConfig());

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

    clearLineTimer->get_elapsed();
    field.render(
        x + w / 3, y + h / 8, w / 3, 3 * h / 4,
        tetrimino, clearLineTimer->get_elapsed() >= CLEAR_LINE_RENDER_TIME
    );

    int blockSize = min(w / TETRIS_FIELD_WIDTH, h / TETRIS_FIELD_HEIGHT);
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
}

bool TetrisLayout::game_over () const
{
    return gameOver;
}
