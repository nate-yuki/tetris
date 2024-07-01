/**
 * @file  game.hpp
 * @brief Include file for Game class.
 */

#ifndef GAME_HPP
#define GAME_HPP


#include "window.hpp"
#include "renderer.hpp"
#include "font.hpp"
#include "gamepad.hpp"
#include "texture.hpp"
#include "key_layout.hpp"
#include "text.hpp"
#include "shapes.hpp"
#include "textbox.hpp"
#include "menu.hpp"
#include "states.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


class GameState;

/**
 * @brief The main class implementing the game logic and rendering
 */
class Game
{
public:
    /**
     * @brief Initialize SDL libraries and class members.
     * @throws `ExceptionSDL` thrown if an SDL library could not be initialized.
     */
    void init();

    /**
     * @brief Handle SDL events.
     * @details
     * Calls `handle_events` on class members. Sets `GameOverState` on `SDL_QUIT`;
     * toggles pause on Escape key press.
     */
    void handle_events();

    /**
     * @brief Do game logic.
     * @details
     * Pauses the game if `window` lost keyboard focus. If the game is not paused,
     * calls `do_logic` on `currState`.
     */
    void do_logic();

    /// Exit `currState` and enter `nextState` if `nextState` is set.
    void change_state();

    /// Do rendering if the window is not minimized.
    void render();

    /// Free SDL libraries and class members.
    void free();

    /// `true` if `currState` is `GameOverState`.
    bool is_over() const;

    /// If `nextState` is not `GameOverState`, set it to `state`.
    void set_next_state(GameState *state);

    /**
     * @brief Load `texture` from the image stored in `path`.
     * @param[out] texture `Texture` object to initialize.
     * @param[in] path Image path.
     * @param[in] keyColor The transparent pixel; `nullptr` to not use color keying.
     */
    void load_texture_from_file(
        Texture &texture, const std::string &path, const Color *keyColor=nullptr
    );

    /**
     * @brief Initialize `text`.
     * @param[out] text `Text` object to initialize.
     * @param[in] line Displayed text.
     * @param[in] color Text color; default is black.
     * @param[in] maxText Text to use for font fitting; by default, `text` is used.
     */
    void create_text(
        Text &text, const std::string &line,
        const Color &color=BLACK, const std::string &maxText=""
    );

    /**
     * @brief Initialize `keyLayout`.
     * @param[out] keyLayout `KeyLayout` object to initialize.
     * @param[in] mapping .
     * @param[in] gamepadInd .
     */
    void create_key_loadout(
        KeyLayout &keyLayout, KeyMap &mapping,
        int gamepadInd=KeyLayout::GamepadSelector::GAMEPAD_NONE
    );

    void create_box(Box &box, const Color &fillColor, const Color &frameColor);
    void create_text_box(
        TextBox &box, const std::string &line,
        const Color &fillColor, const Color &frameColor,
        const Color &textColor=BLACK, const std::string &maxText=""
    );

    /**
     * @brief Initialize `menu`.
     * @param[out] menu `Menu` object to initialize.
     * @param[in] prompt Menu prompt text.
     * @param[in] options Menu option texts in order.
     * @param[in] promptFillColor Fill color for menu prompt box.
     * @param[in] promptFrameColor Frame color for menu prompt box.
     * @param[in] optionFillColor Fill color for unselected option boxes.
     * @param[in] optionFrameColor Frame color for unselected option boxes.
     * @param[in] selectedOptionFillColor Fill color for the selected option box.
     * @param[in] selectedOptionFrameColor Frame color for the selected option box.
     * @param[in] promptTextColor Prompt text color.
     * @param[in] optionTextColor Option text color.
     */
    void create_menu(
        Menu &menu,
        const std::string &prompt, const std::vector<std::string> &options,
        const Color &promptFillColor={128, 128, 128},
        const Color &promptFrameColor=BLACK,
        const Color &optionFillColor={192, 192, 192},
        const Color &optionFrameColor={64, 64, 64},
        const Color &selectedOptionFillColor={224, 224, 224},
        const Color &selectedOptionFrameColor=WHITE,
        const Color &promptTextColor=BLACK, const Color &optionTextColor=BLACK
    );

    /**
     * @brief Display the current scene.
     * @note Used by `Window` when the window is resized.
     * @see Window::handle_event
     */
    void show();

    /// Set renderer view port.
    void set_renderer_view_port(int x, int y, int w, int h);

    /// Reset renderer view port to the whole window.
    void reset_renderer_view_port();

    /// Get renderer output width.
    int get_renderer_width() const;

    /// Get renderer output height.
    int get_renderer_height() const;

    /// `true` if the game is paused.
    bool is_paused() const;

    /**
     * @brief Set the score and the high score.
     * @note Used to transfer the scores between `GameState`s.
     */
    void set_scores(int score, int highScore);

    /**
     * @brief Get the score.
     * @note Used to transfer the score between `GameState`s.
     */
    int get_score() const;

    /**
     * @brief Get the high score.
     * @note Used to transfer the high score between `GameState`s.
     */
    int get_high_score() const;

    /**
     * @brief Set the player amount.
     * @note Used to transfer the player amount between `GameState`s.
     */
    void set_players(int players);

    /**
     * @brief Get the player amount.
     * @note Used to transfer the player amount between `GameState`s.
     */
    int get_players() const;

    /**
     * @brief Set the winner index.
     * @note Used to transfer the winner index between `GameState`s.
     */
    void set_winner(int winner);

    /**
     * @brief Get the winner index.
     * @note Used to transfer the winner index between `GameState`s.
     */
    int get_winner() const;

private:
    void pause();
    void unpause();

    Window window;
    Font font;
    Renderer renderer;
    GamepadManager gamepads;
    GameState *currState, *nextState;
    bool paused;

    int score, highScore;
    int players, winner;
};


#endif
