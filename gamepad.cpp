#include "gamepad.hpp"
#include "logger.hpp"


#include "game.hpp"

#include <string>


void Gamepad::init (int index)
{
    log("Initializing Gamepad " + std::to_string(index), __FILE__, __LINE__);

    if (!SDL_IsGameController(index))
    {
        log(
            "[WARNING] Gamepad " + std::to_string(index)
            + " is not game controller interface compatible! SDL Error: "
            + std::string{SDL_GetError()},
            __FILE__, __LINE__, true
        );
    }
    else
    {
        gameController = SDL_GameControllerOpen(index);
        
        SDL_Joystick *joyTemp = SDL_GameControllerGetJoystick(gameController);
        id = SDL_JoystickInstanceID(joyTemp);
        log(
            "Gamepad " + std::to_string(index) + " got id " + std::to_string(id),
            __FILE__, __LINE__
        );

        if (!SDL_GameControllerHasRumble(gameController))
        {
            log(
                "[WARNING] Gamepad " + std::to_string(index)
                + " does not have rumble! SDL Error: " + std::string{SDL_GetError()},
                __FILE__, __LINE__, true
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
                __FILE__, __LINE__, true
            );
        }
        else
        {
            id = SDL_JoystickInstanceID(joystick);
            log(
                "Gamepad " + std::to_string(index) + " got id " + std::to_string(id),
                __FILE__, __LINE__
            );

            if (!SDL_JoystickIsHaptic(joystick))
            {
                log(
                    "[WARNING] Gamepad " + std::to_string(index)
                    + " does not support haptics! SDL Error: "
                    + std::string{SDL_GetError()},
                    __FILE__, __LINE__, true
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
                        __FILE__, __LINE__, true
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
                            __FILE__, __LINE__, true
                        );
                    }
                }
            }
        }
    }
}

void Gamepad::free ()
{
    log("Freeing gamepad with id " + std::to_string(id), __FILE__, __LINE__);

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

SDL_JoystickID Gamepad::get_id () const
{
    return id;
}

bool Gamepad::is_attached ()
{
    if (gameController != NULL)
    {
        return SDL_GameControllerGetAttached(gameController);
    }
    if (joystick != NULL)
    {
        return SDL_JoystickGetAttached(joystick);
    }
    return false;
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

void GamepadManager::handle_event (Game &game, const SDL_Event &e)
{
    int index, id;
    switch (e.type)
    {
        case SDL_JOYDEVICEADDED:
            index = e.jdevice.which;

            log(
                "A Gamepad was connected and assigned index " + std::to_string(index),
                __FILE__, __LINE__
            );

            if (gamepads.size() < SDL_NumJoysticks())
            {
                if (index == gamepads.size())
                {
                    log(
                        "Appending Gamepad " + std::to_string(index),
                        __FILE__, __LINE__
                    );

                    gamepads.resize(gamepads.size() + 1);
                    gamepads[index].init(index);
                }
                else if (!gamepads[index].is_attached())
                {
                    log(
                        "[WARNING] Inserting Gamepad " + std::to_string(index),
                        __FILE__, __LINE__, true
                    );

                    gamepads.resize(gamepads.size() + 1);
                    for (int i = gamepads.size() - 1; i > index; --i)
                    {
                        std::swap(gamepads[i], gamepads[i - 1]);
                    }
                    gamepads[index].init(index);
                }
            }
            else if (!gamepads[index].is_attached())
            {
                log(
                    "[WARNING] Reinitializing Gamepad " + std::to_string(index),
                    __FILE__, __LINE__, true
                );

                gamepads[index].init(index);
            }
            else
            {
                log(
                    "Gamepad " + std::to_string(index)
                    + " was initialized before the event handling",
                    __FILE__, __LINE__
                );
            }
            break;

        case SDL_JOYDEVICEREMOVED:
            id = e.jdevice.which;

            log(
                "The Gamepad with id " + std::to_string(id) + " was removed",
                __FILE__, __LINE__
            );

            int i;
            for (i = 0; i < gamepads.size(); ++i)
            {
                if (gamepads[i].get_id() == id)
                {
                    break;
                }
            }
            if (i != gamepads.size())
            {
                if (!gamepads[i].is_attached())
                {
                    log(
                        "Gamepad " + std::to_string(i) + " is not attached, removing",
                        __FILE__, __LINE__
                    );

                    gamepads[i].free();
                    for (int j = i; j < gamepads.size() - 1; ++j)
                    {
                        std::swap(gamepads[j], gamepads[j + 1]);
                    }
                    gamepads.resize(gamepads.size() - 1);
                }
                else
                {
                    log(
                        "Gamepad " + std::to_string(i)
                        + " was reattached before the event handling",
                        __FILE__, __LINE__
                    );
                }
            }
            else
            {
                log(
                    "Gamepad with id " + std::to_string(id)
                    + " was not initialized before the event handling",
                    __FILE__, __LINE__
                );
            }
            break;
    }
    if (
        (e.type == SDL_JOYDEVICEADDED || e.type == SDL_JOYDEVICEREMOVED)
        && gamepads.size() != SDL_NumJoysticks()
    )
    {
        log(
            "[WARNING] Gamepads stored " + std::to_string(gamepads.size())
            + " != total joysticks " + std::to_string(SDL_NumJoysticks()),
            __FILE__, __LINE__, true
        );
    }
}

void GamepadManager::vibrate (
    int index, Uint32 duration, Uint16 low_freq, Uint16 high_freq
)
{
    if (index == GAMEPAD_ALL)
    {
        for (int i = 0; i < gamepads.size(); ++i)
        {
            gamepads[i].vibrate(duration, low_freq, high_freq);
        }
    }
    else if (index < gamepads.size())
    {
        gamepads[index].vibrate(duration, low_freq, high_freq);
    }
}
