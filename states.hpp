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
#include "key_layout.hpp"
#include "tetris_layout.hpp"

#include <SDL2/SDL.h>
#include <vector>


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

/// A screen with the title and a prompt to press Enter to go to the next state.
class TitleScreenState: public GameState
{
public:
    static TitleScreenState *get();

    enum Commands{
        START,
    };

    static KeyMap keyMap;

    void enter(Game *game);
    void exit();

    /// Transitions to `MenuState` on Enter press.
    void handle_event(Game &game, const SDL_Event &e);

    void do_logic();
    void render();

    void pause_timers();
    void unpause_timers();
    
private:
    static TitleScreenState sTitleScreenState;
    TitleScreenState();

    Game *game;
    KeyLayout keyLayout;
    Texture bgTexture;
    Text titleText, promptText;
};

/// A menu with options to start the game and to exit.
class MenuState: public GameState
{
public:
    static MenuState *get();

    void enter(Game *game);
    void exit();

    void handle_event(Game &game, const SDL_Event &e);
    void do_logic();
    void render();

    void pause_timers();
    void unpause_timers();
    
private:
    static MenuState sMenuState;
    MenuState();

    Game *game;

    Menu menu;
};

/// A menu with player amount selection.
class PlayersSelectState: public GameState
{
public:
    static PlayersSelectState *get();

    void enter(Game *game);
    void exit();

    void handle_event(Game &game, const SDL_Event &e);
    void do_logic();
    void render();

    void pause_timers();
    void unpause_timers();
    
private:
    static PlayersSelectState sPlayersSelectState;
    PlayersSelectState();

    Game *game;

    Menu menu;
};

/// The main state with the Tetris itself.
class TetrisState: public GameState
{
public:
    static TetrisState *get();

    enum Commands{
        END,
    };

    /// Read the high score.
    void enter(Game *game);

    /// Pass the scores to `Game`. If the high score has changed, write it.
    void exit();
    
    /// Force transition to `ResultsScreenState` on END key press.
    void handle_event(Game &game, const SDL_Event &e);

    /// If the game is over, wait before transitioning to `ResultsScreenState`.
    void do_logic();

    void render();

    void pause_timers();
    void unpause_timers();
    
private:
    static TetrisState sTetrisState;
    TetrisState();

    static KeyMap keyMap, tetrisKeyMap, tetriminoKeyMap;

    Game *game;
    KeyLayout keyLayout, tetrisLayout, tetriminoLayout;
    Texture bgTexture, blockTextureSheet;
    Texture fieldBgTexture, fieldFrameTexture, fieldClearTexture;
    Texture fieldClearParticleTextureSheet;
    Text linesClearedText, linesClearedPromptText;
    Text scoreText, scorePromptText, highScoreText, highScorePromptText;
    Text msgText, comboText;
    Timer tetriminoTimer, clearLineTimer, msgTextTimer, gameOverTimer;
    TetrisLayout tetris;

    int highScore;
};

/// Multiple players tetris state.
class TetrisPVPState: public GameState
{
public:
    static TetrisPVPState *get();

    enum Commands{
        END,
    };

    void enter(Game *game);

    /// Pass the scores to `Game`. If the high score has changed, write it.
    void exit();
    
    /// Force transition to `ResultsScreenState` on END key press.
    void handle_event(Game &game, const SDL_Event &e);

    /// If the game is over, wait before transitioning to `ResultsScreenState`.
    void do_logic();

    void render();

    void pause_timers();
    void unpause_timers();
    
private:
    static TetrisPVPState sTetrisPVPState;
    TetrisPVPState();

    static KeyMap keyMap;
    static std::vector<KeyMap> tetrisKeyMaps, tetriminoKeyMaps;

    Game *game;
    KeyLayout keyLayout;
    std::vector<KeyLayout> tetrisLayouts, tetriminoLayouts;
    Texture bgTexture, blockTextureSheet;
    Texture fieldBgTexture, fieldFrameTexture, fieldClearTexture;
    Texture fieldClearParticleTextureSheet;
    Text linesClearedPromptText, scorePromptText, highScorePromptText;
    std::vector<Text> linesClearedTexts, scoreTexts, highScoreTexts;
    std::vector<Text> msgTexts, comboTexts;
    std::vector<Timer> tetriminoTimers, clearLineTimers, msgTextTimers;
    std::vector<Timer> gameOverTimers;
    std::vector<TetrisLayout> tetris;

    int players;
};

/// A screen displaying the high score or the winner and the score.
class ResultsScreenState: public GameState
{
public:
    static ResultsScreenState *get();

    enum Commands{
        SKIP,
    };

    /// Get the score from `game` and construct messages.
    void enter(Game *game);

    void exit();

    /// Force transition to a menu state on Enter key press.
    void handle_event(Game &game, const SDL_Event &e);

    /// Wait before transitioning to a menu state.
    void do_logic();

    void render();

    void pause_timers();
    void unpause_timers();

private:
    static ResultsScreenState sResultsScreenState;
    ResultsScreenState();

    static KeyMap keyMap;

    Game *game;
    KeyLayout keyLayout;
    Texture bgTexture;
    Text titleText, resultsText;

    Timer resultsTimer; /// Halts automatic transition to `MenuState`.
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
