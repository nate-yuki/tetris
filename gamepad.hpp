/**
 * @file  gamepad.hpp
 * @brief Include file for Gamepad and GamepadManager classes.
 */

#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP


#include <SDL2/SDL.h>
#include <vector>


class Game;

/**
 * @brief A gamepad wrapper class with support for both `SDL_GameController` and
 *     `SDL_Joystick`.
 */
class Gamepad
{
public:
    /**
     * @brief Initialize the gamepad and store the instance id.
     * @param index The joystick device index.
     */
    void init(int index);

    /// Free the gamepad.
    void free();

    /// Get the instance id.
    SDL_JoystickID get_id() const;

    /// `true` if the gamepad was initialized and is currently connected.
    bool is_attached();

    /**
     * @brief Start a rumble effect on the gamepad.
     * @details
     * Each call cancels any previous rumble effect.
     * Setting `duration` to `0` stops any rumbling.
     * @param duration Vibration duration in ms; default is `0`.
     * @param low_freq The intensity of the low frequency (left) motor, from `0` to
     *     `0xFFFF`; default is `0xFFFF`.
     * @param high_freq he intensity of the high frequency (right) motor, from `0` to
     *     `0xFFFF`; default is `0xFFFF`.
     */
    void vibrate(Uint32 duration=0, Uint16 low_freq=0xFFFF, Uint16 high_freq=0xFFFF);
    
    /// `true` if the gamepad was initialized and the `button`th button is pressed.
    bool button_pressed(int button);

private:
    SDL_JoystickID id; // The instance id.
    SDL_GameController *gameController = NULL;
    SDL_Joystick *joystick = NULL;
    SDL_Haptic *joyHaptic = NULL;
};

/// A class for simple management of all gamepads currently attached to the system.
class GamepadManager
{
public:
    /// A flag to select every gamepad.
    static constexpr int GAMEPAD_ANY = -1, GAMEPAD_ALL = -1;

    /// Initialize as many gamepads as are attached to the system.
    void init();

    /// Free all gamepads.
    void free();

    /// Handle connection and disconnection of gamepads.
    void handle_event(Game &game, const SDL_Event &e);

    /**
     * @brief Get the instance id of a gamepad.
     * @param index The joystick device index of the gamepad.
     * @return The instance id or `-1` if `index` is larger than the number of
     *     stored gamepads.
     */
    SDL_JoystickID get_id(int index) const;

    /**
     * @brief Start a rumble effect on a gamepad.
     * @details
     * Each call cancels any previous rumble effect.
     * Setting `duration` to `0` stops any rumbling.
     * @note If `index` is larger than the number of stored gamepads, does nothing.
     * @param index The joystick device index of the gamepad, or
     *     `GamepadManager::GAMEPAD_ALL` to select every gamepad; default is
     *     `GamepadManager::GAMEPAD_ALL`.
     * @param duration Vibration duration in ms; default is `0`.
     * @param low_freq The intensity of the low frequency (left) motor, from 0 to
     *     0xFFFF; default is 0xFFFF.
     * @param high_freq he intensity of the high frequency (right) motor, from 0 to
     *     0xFFFF; default is 0xFFFF.
     */
    void vibrate(
        int index=GAMEPAD_ALL,
        Uint32 duration=0, Uint16 low_freq=0xFFFF, Uint16 high_freq=0xFFFF
    );

    /**
     * @brief Check if `button` is pressed on the gamepad with index `index`.
     * @note If `index` is larger than the number of stored gamepads, does nothing.
     * @param index The joystick device index of the gamepad, or
     *     `GamepadManager::GAMEPAD_ANY` to check every gamepad.
     * @param button The button index.
     * @return `true` if a gamepad was initialized and the button with index `button`
     *     is pressed.
     */
    bool button_pressed(int index, int button);

private:
    std::vector<Gamepad> gamepads;
};


#endif
