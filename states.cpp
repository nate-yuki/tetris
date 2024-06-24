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
    game->create_text(titleText, "TETRIS", WHITE);
    game->create_text(promptText, "(press Enter to start)", WHITE);
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
    
    game->load_texture_from_file(bgTexture, "textures/bg.png");
    game->load_texture_from_file(blockTextureSheet, "textures/blocks.png", &CYAN);
    game->load_texture_from_file(fieldBgTexture, "textures/field_bg.png");
    game->load_texture_from_file(fieldFrameTexture, "textures/field_frame.png");
    game->load_texture_from_file(fieldClearTexture, "textures/field_clear.png");
    game->load_texture_from_file(
        fieldClearParticleTextureSheet, "textures/field_particles.png", &CYAN
    );

    game->create_text(linesClearedText, "0000", WHITE, "999999999");
    game->create_text(linesClearedPromptText, "Lines cleared:", WHITE);
    game->create_text(scoreText, "000000000", WHITE, "999999999");
    game->create_text(scorePromptText, "Score:", WHITE, "High score:");
    game->create_text(highScoreText, "042424242", WHITE, "999999999");
    game->create_text(highScorePromptText, "High score:", WHITE);
    game->create_text(msgText, "", WHITE, std::string(32, 'W'));
    game->create_text(comboText, "Combo: 0", WHITE, "Combo: 99");

    tetris.init(
        TETRIS_FIELD_WIDTH, TETRIS_FIELD_HEIGHT,
        &tetriminoTimer, &clearLineTimer, &gameOverTimer,
        &msgTextTimer,
        &bgTexture, &blockTextureSheet,
        &fieldBgTexture, &fieldFrameTexture, &fieldClearTexture,
        &fieldClearParticleTextureSheet,
        &linesClearedText, &linesClearedPromptText,
        &scoreText, &scorePromptText,
        &highScoreText, &highScorePromptText,
        &msgText, &comboText
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
    linesClearedPromptText.free();
    scoreText.free();
    scorePromptText.free();
    highScoreText.free();
    highScorePromptText.free();
    msgText.free();
    comboText.free();

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
    msgTextTimer.pause();
}

void TetrisState::unpause_timers ()
{
    tetriminoTimer.unpause();
    clearLineTimer.unpause();
    gameOverTimer.unpause();
    msgTextTimer.unpause();
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
