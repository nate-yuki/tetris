/**
 * @file  states.cpp
 * @brief Implementation of the game state classes.
 */

#include "states.hpp"
#include "constants.hpp"
#include "logger.hpp"


// Singleton instances

TitleScreenState TitleScreenState::sTitleScreenState;
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
}

void TitleScreenState::exit ()
{
    log("Exiting TitleScreen", __FILE__, __LINE__);

    bgTexture.free();
}

void TitleScreenState::handle_event (Game &game, const SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && !game.is_paused())
    {
        game.set_next_state(GameOverState::get());
    }
}

void TitleScreenState::do_logic () {}

void TitleScreenState::render ()
{
    bgTexture.render(
        (game->get_renderer_width() - bgTexture.get_width()) / 2,
        (game->get_renderer_height() - bgTexture.get_height()) / 2
    );
}

void TitleScreenState::pause_timers () {}
void TitleScreenState::unpause_timers () {}


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
