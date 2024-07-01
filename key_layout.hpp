#ifndef KEY_LOADOUT_HPP
#define KEY_LOADOUT_HPP


#include "gamepad.hpp"

#include <SDL2/SDL.h>
#include <map>
#include <set>


using KeyMap = std::map<int, const std::set<int>>;


class Game;

class KeyLayout
{
public:
    enum GamepadSelector{
        GAMEPAD_ANY = GamepadManager::GAMEPAD_ANY,
        GAMEPAD_NONE = GAMEPAD_ANY - 1,
    };

    enum EventType{
        NONE,
        DOWN,
        UP,
    };

    static constexpr int GP_CODE_SEP = SDLK_ENDCALL + 1;

    void init(
        KeyMap &mapping, GamepadManager *gamepads, int gamepadInd=GAMEPAD_NONE
    );
    void handle_event(Game &game, const SDL_Event &e);
    EventType get_type() const;
    int get_map() const;
    Uint8 get_repeat() const;
    void store_pressed();
    bool pressed(int keyMap) const;

private:
    KeyMap mapping;
    GamepadManager *gamepads;
    int gamepadInd;
    Uint32 eventType;
    int code;
    int map;
    Uint8 repeat;
    const Uint8 *keys;
    std::set<int> pressedKeyMaps;
};


#endif
