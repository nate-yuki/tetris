/**
 * @file  game.cpp
 * @brief Implementation of the class Game.
 */

#include "game.hpp"
#include "audio.hpp"
#include "particles.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "logger.hpp"


KeyMap Game::keyMap{
    {
        Game::Commands::PAUSE,
        {SDLK_ESCAPE, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_START}
    },
    {
        Game::Commands::SOUND_TOGGLE,
        {SDLK_m, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_BACK}
    },
};


void Game::init ()
{
    log("Initializing Game", __FILE__, __LINE__);

    // Initialize SDL libraries
    if (
        SDL_Init(
            SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC |
            SDL_INIT_GAMECONTROLLER
        ) < 0
    )
    {
        throw ExceptionSDL(__FILE__, __LINE__, SDL_GetError());
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        log(
            "[WARNING] Linear texture filtering not enabled!",
            __FILE__, __LINE__, true
        );
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        throw ExceptionSDL(__FILE__, __LINE__, IMG_GetError());
    }
    if (TTF_Init() == -1)
    {
        throw ExceptionSDL(__FILE__, __LINE__, TTF_GetError());
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        throw ExceptionSDL(__FILE__, __LINE__, Mix_GetError());
    }

    // Initialize audio
    Audio::init();

    // Initialize class members
    window.init(*this);
    renderer.init(window);
    font.init("fonts/font.ttf", 30);
    gamepads.init();

    create_key_loadout(keyLayout, keyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY);

    // Initialize particles
    Particle::init_clips();

    // Initialize tetrimino
    Tetrimino::load_schemes("schemes.txt");
    Tetrimino::init_clips();

    paused = false;

    // Begin state rotation
    currState = TitleScreenState::get();
    nextState = nullptr;
    currState->enter(this);
}

void Game::handle_events ()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            set_next_state(GameOverState::get());
        }

        window.handle_event(*this, e);

        keyLayout.handle_event(*this, e);
        if (keyLayout.get_type() == KeyLayout::DOWN && keyLayout.get_repeat() == 0)
        {
            switch (keyLayout.get_command())
            {
            case PAUSE:
                if (paused)
                {
                    unpause();
                }
                else
                {
                    pause();
                }
                break;
            case SOUND_TOGGLE:
                if (Audio::toggle_sound())
                {
                    Audio::play_sound(Audio::UNMUTE);
                }
                break;
            }
        }

        gamepads.handle_event(*this, e);
        currState->handle_event(*this, e);
    }
}

void Game::do_logic ()
{
    if (!window.has_keyboard_focus() && !paused)
    {
        pause();
    }
    if (!paused)
    {
        currState->do_logic();
    }
}

void Game::change_state ()
{
    // nextState == nullptr means the next state is not set
    if (nextState != nullptr)
    {
        currState->exit();
        nextState->enter(this);

        currState = nextState;
        nextState = nullptr;
    }
}

void Game::render ()
{
    if (!window.is_minimized())
    {
        renderer.clear();
        currState->render();
        renderer.show();
    }
}

void Game::free ()
{
    log("Freeing Game", __FILE__, __LINE__);

    if (nextState != nullptr) // If a crash happed during entering next state
    {
        nextState->exit();
    }
    else
    {
        currState->exit();
    }
    
    window.free();
    renderer.free();
    font.free();
    gamepads.free();

    Audio::free();

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool Game::is_over () const
{
    return currState == GameOverState::get();
}

void Game::set_next_state (GameState *state)
{
    log("Setting next state", __FILE__, __LINE__);

    if (nextState != GameOverState::get())
    {
        nextState = state;
    }
}

void Game::load_texture_from_file (
    Texture &texture, const std::string &path, const Color *keyColor
)
{
    texture.load_from_file(renderer, path, keyColor);
}

void Game::create_text (
    Text &text, const std::string &line, const Color &color,
    const std::string &maxText
)
{
    text.init(renderer, font, line, color, maxText);
}

void Game::create_key_loadout (KeyLayout &keyLayout, KeyMap &mapping, int gamepadInd)
{
    keyLayout.init(mapping, &gamepads, gamepadInd);
}

void Game::create_box (Box &box, const Color &fillColor, const Color &frameColor)
{
    box.init(renderer, fillColor, frameColor);
}

void Game::create_text_box (
    TextBox &box, const std::string &line,
    const Color &fillColor, const Color &frameColor, const Color &textColor,
    const std::string &maxText
)
{
    box.init(renderer, font, line, fillColor, frameColor, textColor, maxText);
}

void Game::create_menu (
    Menu &menu, const std::string &prompt, const std::vector<std::string> &options,
    const Color &promptFillColor, const Color &promptFrameColor,
    const Color &optionFillColor, const Color &optionFrameColor,
    const Color &selectedOptionFillColor, const Color &selectedOptionFrameColor,
    const Color &promptTextColor, const Color &optionTextColor
)
{
    menu.init(
        *this, prompt, options,
        promptFillColor, promptFrameColor, optionFillColor, optionFrameColor,
        selectedOptionFillColor, selectedOptionFrameColor,
        promptTextColor, optionTextColor
    );
}

void Game::show ()
{
    renderer.show();
}

void Game::set_renderer_view_port (int x, int y, int w, int h)
{
    renderer.set_view_port(x, y, w, h);
}

void Game::reset_renderer_view_port ()
{
    renderer.reset_view_port();
}

int Game::get_renderer_width () const
{
    return renderer.get_width();
}

int Game::get_renderer_height () const
{
    return renderer.get_height();
}

bool Game::is_paused () const
{
    return paused;
}

void Game::set_scores (int score, int highScore)
{
    this->score = score;
    this->highScore = highScore;
}

int Game::get_score () const
{
    return score;
}

int Game::get_high_score () const
{
    return highScore;
}

void Game::set_players (int players)
{
    this->players = players;
}

int Game::get_players () const
{
    return players;
}

void Game::set_winner (int winner)
{
    this->winner = winner;
}

int Game::get_winner () const
{
    return winner;
}

void Game::pause ()
{
    currState->pause_timers();
    paused = true;

    Audio::play_sound(Audio::GAME_PAUSE);
    Audio::pause_music();
}

void Game::unpause ()
{
    currState->unpause_timers();
    paused = false;

    Audio::play_sound(Audio::GAME_UNPAUSE);
    Audio::unpause_music();
}
