#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP


#include <SDL2/SDL.h>
#include <vector>


class Gamepad
{
public:
    void init(int ind);
    void free();
    void vibrate(Uint32 duration=0, Uint16 low_freq=0xFFFF, Uint16 high_freq=0xFFFF);

private:
    int index;
    SDL_GameController *gameController = NULL;
    SDL_Joystick *joystick = NULL;
    SDL_Haptic *joyHaptic = NULL;
};


class GamepadManager
{
public:
    void init();
    void free();
    void vibrate (
        int index=-1,
        Uint32 duration=0, Uint16 low_freq=0xFFFF, Uint16 high_freq=0xFFFF
    );

private:
    std::vector<Gamepad> gamepads;
};


#endif
