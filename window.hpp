/**
 * @file  window.hpp
 * @brief Include file for Window class.
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP


#include "key_layout.hpp"

#include <SDL2/SDL.h>


class Game;

/// SDL_Window wrapper class.
class Window
{
public:
    enum Commands{
        FULL_SCREEN_TOGGLE,
    };

    /**
     * @brief Create the window.
     * @throws `ExceptionSDL` thrown if the window could not be created.
     */
    void init(Game &game);

    /// Destroy the window if it was not destroyed.
    void free();

    /**
     * @brief Create an SDL renderer attached to the window.
     * @return A pointer to the created renderer.
     * @throws `ExceptionSDL` thrown if the renderer could not be created.
     */
    SDL_Renderer *create_renderer();

    /**
     * @brief Handle SDL window events.
     * @details
     * On resize or being exposed, displays the current scene through `game`.
     * On F11 key press, toggles full screen.
     */
    void handle_event(Game &game, const SDL_Event &e);

    /// Get window width.
    int get_width() const;

    /// Get window height.
    int get_height() const;
    
    bool has_mouse_focus() const;
    bool has_keyboard_focus() const;
    bool is_minimized() const;

private:
    static KeyMap keyMap;

    SDL_Window *window = NULL;
    KeyLayout keyLayout;
    int w, h;
    bool mouseFocus;
    bool keyboardFocus;
    bool minimized;
    bool fullScreen;
};


#endif
