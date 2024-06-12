/**
 * @file  states.hpp
 * @brief Include file for game state classes.
 */

#ifndef STATES_HPP
#define STATES_HPP


#include "game.hpp"
#include "texture.hpp"
#include "text.hpp"
#include "timer.hpp"
#include "menu.hpp"

#include <SDL2/SDL.h>


class Game;

/// Virtual class for game states.
class GameState
{
public:
    /// Load game state objects. Store pointer to `Game`.
    virtual void enter(Game *game) = 0;

    /// Free game state objects.
    virtual void exit() = 0;

    /// Handle SDL events.
    virtual void handle_event(Game &game, const SDL_Event &e) = 0;

    /// Do game logic.
    virtual void do_logic() = 0;

    /// Render the scene without clearing the previous one.
    virtual void render() = 0;

    /// Pause all timers.
    virtual void pause_timers() = 0;

    /// Unpause all timers.
    virtual void unpause_timers() = 0;

    virtual ~GameState(){};
};

class TitleScreenState: public GameState
{
public:
    static TitleScreenState *get();

    void enter(Game *game);
    void exit();

    void handle_event(Game &game, const SDL_Event &e);

    void do_logic();
    void render();

    void pause_timers();
    void unpause_timers();
    
private:
    static TitleScreenState sTitleScreenState;
    TitleScreenState();

    Game *game;
    Texture bgTexture;
};

/// A utility state used only to indicate the game over.
class GameOverState: public GameState
{
public:
    static GameOverState *get();

    void enter(Game *game);
    void exit();

    void handle_event(Game &game, const SDL_Event &e);
    void do_logic();
    void render();

    void pause_timers();
    void unpause_timers();

private:
    static GameOverState sGameOverState;
    GameOverState();
};


#endif
