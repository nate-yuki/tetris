/**
 * @file  window.cpp
 * @brief Implementation of the class Window.
 */

#include "window.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "logger.hpp"


KeyMap Window::keyMap{
    {
        Window::Commands::FULL_SCREEN_TOGGLE,
        {SDLK_F11, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_LEFTSTICK}
    },
};


void Window::init (Game &game)
{
    log("Initializing Window", __FILE__, __LINE__);

    window = SDL_CreateWindow(
        "Tetris",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (window == NULL)
    {
        throw ExceptionSDL(__FILE__, __LINE__, SDL_GetError());
    }

    game.create_key_loadout(
        keyLayout, keyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY
    );

    w = WINDOW_WIDTH;
    h = WINDOW_HEIGHT;
    mouseFocus = true;
    keyboardFocus = true;
}

void Window::free ()
{
    log("Freeing window", __FILE__, __LINE__);
    
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

SDL_Renderer *Window::create_renderer ()
{
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED// | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == NULL)
    {
        throw ExceptionSDL(__FILE__, __LINE__, SDL_GetError());
    }

    return renderer;
}

void Window::handle_event (Game &game, const SDL_Event &e)
{
    if (e.type == SDL_WINDOWEVENT)
    {
        switch (e.window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            w = e.window.data1;
            h = e.window.data2;
            game.show();
            break;

        case SDL_WINDOWEVENT_EXPOSED:
            game.show();
            break;

        case SDL_WINDOWEVENT_ENTER:
            mouseFocus = true;
            break;

        case SDL_WINDOWEVENT_LEAVE:
            mouseFocus = false;
            break;

        case SDL_WINDOWEVENT_FOCUS_GAINED:
            keyboardFocus = true;
            break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
            keyboardFocus = false;
            break;

        case SDL_WINDOWEVENT_MINIMIZED:
            minimized = true;
            break;

        case SDL_WINDOWEVENT_MAXIMIZED:
            minimized = false;
            break;

        case SDL_WINDOWEVENT_RESTORED:
            minimized = false;
            break;
        }
    }
    else
    {
        keyLayout.handle_event(game, e);
        if (keyLayout.get_type() == KeyLayout::DOWN)
        {
            switch (keyLayout.get_command())
            {
            case FULL_SCREEN_TOGGLE:
                if (fullScreen)
                {
                    SDL_SetWindowFullscreen(window, 0);
                    fullScreen = false;
                }
                else
                {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    fullScreen = true;
                    minimized = false;
                }
            }
        }
    }
}

int Window::get_width () const
{
    return w;
}

int Window::get_height () const
{
    return h;
}

bool Window::has_mouse_focus() const
{
    return mouseFocus;
}

bool Window::has_keyboard_focus() const
{
    return keyboardFocus;
}

bool Window::is_minimized() const
{
    return minimized;
}
