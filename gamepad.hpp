#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP


#include <SDL2/SDL.h>
#include <vector>


class Game;

class Gamepad
{
public:
    void init(int ind);
    void free();
    SDL_JoystickID get_id() const;
    bool is_attached();
    void vibrate(Uint32 duration=0, Uint16 low_freq=0xFFFF, Uint16 high_freq=0xFFFF);
    bool button_pressed(int button);

private:
    SDL_JoystickID id;
    SDL_GameController *gameController = NULL;
    SDL_Joystick *joystick = NULL;
    SDL_Haptic *joyHaptic = NULL;
};


class GamepadManager
{
public:
    static constexpr int GAMEPAD_ANY = -1, GAMEPAD_ALL = -1;

    void init();
    void free();
    void handle_event(Game &game, const SDL_Event &e);
    void vibrate (
        int index=GAMEPAD_ALL,
        Uint32 duration=0, Uint16 low_freq=0xFFFF, Uint16 high_freq=0xFFFF
    );

private:
    std::vector<Gamepad> gamepads;
};


#endif
