#include "gamepad.hpp"
#include "logger.hpp"

#include <string>


void Gamepad::init (int index)
{
    log("Initializing gamepad " + std::to_string(index), __FILE__, __LINE__);

    if (!SDL_IsGameController(index))
    {
        log(
            "[WARNING] Gamepad " + std::to_string(index)
            + " is not game controller interface compatible! SDL Error: "
            + std::string{SDL_GetError()},
            __FILE__, __LINE__
        );
    }
    else
    {
        gameController = SDL_GameControllerOpen(index);
        if (!SDL_GameControllerHasRumble(gameController))
        {
            log(
                "[WARNING] Gamepad " + std::to_string(index)
                + " does not have rumble! SDL Error: " + std::string{SDL_GetError()},
                __FILE__, __LINE__
            );
        }
    }

    // Load joystick if game controller could not be loaded
    if (gameController == NULL)
    {
        joystick = SDL_JoystickOpen(index);
        if (joystick == NULL)
        {
            log(
                "[WARNING] Unable to open joystick " + std::to_string(index)
                + "! SDL Error: " + std::string{SDL_GetError()},
                __FILE__, __LINE__
            );
        }
        else
        {
            if (!SDL_JoystickIsHaptic(joystick))
            {
                log(
                    "[WARNING] Gamepad " + std::to_string(index)
                    + " does not support haptics! SDL Error: "
                    + std::string{SDL_GetError()},
                    __FILE__, __LINE__
                );
            }
            else
            {
                joyHaptic = SDL_HapticOpenFromJoystick(joystick);
                if (joyHaptic == NULL)
                {
                    log(
                        "[WARNING] Unable to get Gamepad " + std::to_string(index)
                        + " haptics! SDL Error: " + std::string{SDL_GetError()},
                        __FILE__, __LINE__
                    );
                }
                else
                {
                    if (SDL_HapticRumbleInit(joyHaptic) < 0)
                    {
                        log(
                            "[WARNING] Unable to initialize Gamepad "
                            + std::to_string(index) + " haptic rumble! SDL Error: "
                            + std::string{SDL_GetError()},
                            __FILE__, __LINE__
                        );
                    }
                }
            }
        }
    }
    this->index = index;
}

void Gamepad::free ()
{
    log("Freeing gamepad " + std::to_string(index), __FILE__, __LINE__);

    if (gameController != NULL)
    {
        SDL_GameControllerClose(gameController);
        gameController = NULL;
    }
    if (joyHaptic != NULL)
    {
        SDL_HapticClose(joyHaptic);
        joyHaptic = NULL;
    }
    if (joystick != NULL)
    {
        SDL_JoystickClose(joystick);
        joystick = NULL;
    }
}

void Gamepad::vibrate (Uint32 duration, Uint16 low_freq, Uint16 high_freq)
{
    if (gameController != NULL)
    {
        SDL_GameControllerRumble(gameController, low_freq, high_freq, duration);
    }
    else if (joyHaptic != NULL)
    {
        SDL_HapticRumblePlay(
            joyHaptic, static_cast<float>(low_freq) / 0xFFFF, duration
        );
    }
}


void GamepadManager::init ()
{
    gamepads.resize(SDL_NumJoysticks());
    for (int i = 0; i < gamepads.size(); ++i)
    {
        gamepads[i].init(i);
    }
}

void GamepadManager::free ()
{
    for (int i = 0; i < gamepads.size(); ++i)
    {
        gamepads[i].free();
    }
}

void GamepadManager::vibrate (
    int index, Uint32 duration, Uint16 low_freq, Uint16 high_freq
)
{
    if (index == -1)
    {
        for (int i = 0; i < gamepads.size(); ++i)
        {
            gamepads[i].vibrate(duration, low_freq, high_freq);
        }
    }
    else
    {
        gamepads[index].vibrate(duration, low_freq, high_freq);
    }
}
