/**
 * @file  states.cpp
 * @brief Implementation of the game state classes.
 */

#include "states.hpp"
#include "util.hpp"
#include "constants.hpp"
#include "logger.hpp"


// Singleton instances

TitleScreenState TitleScreenState::sTitleScreenState;
TetrisState TetrisState::sTetrisState;
GameOverState GameOverState::sGameOverState;


TitleScreenState::TitleScreenState () {}

TitleScreenState *TitleScreenState::get ()
{
    return &sTitleScreenState;
}

void TitleScreenState::enter (Game *game)
{
    log("Entering TitleScreen", __FILE__, __LINE__);

    this->game = game;
    
    game->load_texture_from_file(bgTexture, "textures/bg.png");
    game->create_text(titleText, "TETRIS", {255, 255, 255});
    game->create_text(promptText, "(press Enter to start)", {255, 255, 255});
}

void TitleScreenState::exit ()
{
    log("Exiting TitleScreen", __FILE__, __LINE__);

    bgTexture.free();
    titleText.free();
    promptText.free();
}

void TitleScreenState::handle_event (Game &game, const SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && !game.is_paused())
    {
        game.set_next_state(TetrisState::get());
    }
}

void TitleScreenState::do_logic () {}

void TitleScreenState::render ()
{
    bgTexture.render(
        (game->get_renderer_width() - bgTexture.get_width()) / 2,
        (game->get_renderer_height() - bgTexture.get_height()) / 2
    );

    titleText.render(
        0, 0, game->get_renderer_width(), game->get_renderer_height() / 2,
        Text::TEXT_CENTER_BOTTOM
    );
    promptText.render(
        0, game->get_renderer_height() / 2,
        game->get_renderer_width(), game->get_renderer_height() / 2,
        Text::TEXT_CENTER_TOP
    );
}

void TitleScreenState::pause_timers () {}
void TitleScreenState::unpause_timers () {}


TetrisState::TetrisState () {}

TetrisState *TetrisState::get ()
{
    return &sTetrisState;
}

void TetrisState::enter (Game *game)
{
    log("Entering Tetris", __FILE__, __LINE__);

    this->game = game;
    
    Color keyColor = {0, 255, 255};
    game->load_texture_from_file(bgTexture, "textures/bg.png");
    game->load_texture_from_file(blockTextureSheet, "textures/blocks.png", &keyColor);
    game->load_texture_from_file(fieldBgTexture, "textures/field_bg.png");
    game->load_texture_from_file(fieldFrameTexture, "textures/field_frame.png");
    game->load_texture_from_file(fieldClearTexture, "textures/field_clear.png");
    game->load_texture_from_file(
        fieldClearParticleTextureSheet, "textures/field_particles.png", &keyColor
    );

    game->create_text(linesClearedText, "0", {255, 255, 255}, "9999");

    tetris.init(
        TETRIS_FIELD_WIDTH, TETRIS_FIELD_HEIGHT,
        &tetriminoTimer, &clearLineTimer, &gameOverTimer,
        &bgTexture, &blockTextureSheet,
        &fieldBgTexture, &fieldFrameTexture, &fieldClearTexture,
        &fieldClearParticleTextureSheet,
        &linesClearedText
    );
    tetriminoTimer.start();
}

void TetrisState::exit ()
{
    log("Exiting Tetris", __FILE__, __LINE__);

    bgTexture.free();
    blockTextureSheet.free();
    fieldBgTexture.free();
    fieldFrameTexture.free();
    fieldClearTexture.free();
    fieldClearParticleTextureSheet.free();

    linesClearedText.free();

    tetris.free();
}

void TetrisState::handle_event (Game &game, const SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_END && !game.is_paused())
    {
        game.set_next_state(GameOverState::get());
    }
    tetris.handle_event(game, e);
}

void TetrisState::do_logic ()
{
    if (tetris.game_over())
    {
        if (gameOverTimer.get_elapsed() >= 1500)
        {
            game->set_next_state(GameOverState::get());
        }
    }
    else
    {
        tetris.do_logic();
    }
}

void TetrisState::render ()
{
    tetris.render(0, 0, game->get_renderer_width(), game->get_renderer_height());
}

void TetrisState::pause_timers ()
{
    tetriminoTimer.pause();
    clearLineTimer.pause();
    gameOverTimer.pause();
}

void TetrisState::unpause_timers ()
{
    tetriminoTimer.unpause();
    clearLineTimer.unpause();
    gameOverTimer.unpause();
}


GameOverState::GameOverState () {}

GameOverState *GameOverState::get ()
{
    return &sGameOverState;
}

void GameOverState::enter (Game *game)
{
    log("Entering GameOver", __FILE__, __LINE__);
}

void GameOverState::exit ()
{
    log("Exiting GameOver", __FILE__, __LINE__);
}

void GameOverState::handle_event (Game &game, const SDL_Event &e) {}
void GameOverState::do_logic () {}
void GameOverState::render () {}
void GameOverState::pause_timers () {}
void GameOverState::unpause_timers () {}
