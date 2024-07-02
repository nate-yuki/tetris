/**
 * @file  key_layout.hpp
 * @brief Include file for KeyLayout class.
 */

#ifndef KEY_LOADOUT_HPP
#define KEY_LOADOUT_HPP


#include "gamepad.hpp"

#include <SDL2/SDL.h>
#include <map>
#include <set>


/**
 * @brief (Command, SDL key codes) pairs;
 * @note KeyLayout::GP_CODE_SEP must be added to `SDL_GameControllerButton`s.
 */
using KeyMap = std::map<int, const std::set<int>>;


class Game;

/**
 * @brief A class for simple input handling from both the keyboard and gamepads.
 * @example
 * Creation:
 * 
 *     enum Commands{
 *         A,
 *         B,
 *     };
 *     KeyMap keyMap{
 *         {
 *             A,
 *             {SDLK_a, SDLK_RETURN, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A}
 *         },
 *         {
 *             B,
 *             {SDLK_b, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_B}
 *         },
 *     };
 *     KeyLayout keyLayout;
 *     game->create_key_loadout(keyLayout, keyMap, 1);
 * 
 * Event handling:
 * 
 *     keyLayout.handle_event(game, e);
 *     if (keyLayout.get_type() == KeyLayout::DOWN && keyLayout.get_repeat() == 0)
 *     {
 *         switch (keyLayout.get_command())
 *         {
 *         case A:
 *             // Do logic for A
 *             break;
 *         case B:
 *             // Do logic for B
 *             break;
 *         }
 *     }
 * 
 * Getting key states:
 * 
 *     keyLayout->store_pressed();
 *     if (keyLayout->pressed(A))
 *     {
 *         // Do logic for A
 *     }
 *     if (keyLayout->pressed(B))
 *     {
 *         // Do logic for B
 *     }
 */
class KeyLayout
{
public:
    /// Gamepad to get input from.
    enum GamepadSelector{
        GAMEPAD_ANY = GamepadManager::GAMEPAD_ANY, // Get input from every gamepad.
        GAMEPAD_NONE = GAMEPAD_ANY - 1, // Do not use a gamepad.
    };

    /// Input event type.
    enum EventType{
        NONE, // Not a supported input event.
        DOWN, // Key press.
        UP, // Key release.
    };

    /// Value to add to `SDL_GameControllerButton`s to separate from `SDL_KeyCode`s.
    static constexpr int GP_CODE_SEP = SDLK_ENDCALL + 1;

    /**
     * @brief Initialize class members.
     * @param mapping The mappings for commands.
     * @param gamepads A `GamepadManager` object to use for functionality support.
     * @param gamepadInd Gamepad to get input from; set to
     *     `KeyLayout::GamepadSelector::GAMEPAD_ANY` to get input from every gamepad;
     *     by default does not use a gamepad.
     */
    void init(
        KeyMap &mapping, GamepadManager *gamepads, int gamepadInd=GAMEPAD_NONE
    );

    /// On supported input event, store the event type and the mapped command.
    void handle_event(Game &game, const SDL_Event &e);

    /// Get the last handled event type.
    EventType get_type() const;

    /**
     * @brief Get the command mapped to the last handled event key code.
     * @return The command or `-1` if there was no trigger.
     */
    int get_command() const;

    /// Get the key repeat from the last handled event.
    Uint8 get_repeat() const;

    /// Store all commands which have their corresponding key currently pressed.
    void store_pressed();

    /// `true` if `command` had its key pressed at the last `store_pressed()` call.
    bool pressed(int command) const;

private:
    KeyMap mapping; // The mapping of commands to key codes.
    GamepadManager *gamepads;
    int gamepadInd; // The index of the gamepad to get input from.
    Uint32 eventType;
    int code; // The last key code.
    int command; // The last triggered command.
    Uint8 repeat;
    std::set<int> pressedKeyMaps;
};


#endif
