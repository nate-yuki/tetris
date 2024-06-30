#ifndef KEY_LOADOUT_HPP
#define KEY_LOADOUT_HPP


#include "game.hpp"

#include <SDL2/SDL.h>
#include <map>
#include <set>


using KeyMap = std::map<int, const std::set<SDL_Keycode>>;


class KeyLayout
{
public:
    KeyLayout (KeyMap &&mapping);
    void handle_event(Game &game, const SDL_Event &e);
    Uint32 get_type() const;
    int get_map() const;
    Uint8 get_repeat() const;
    void store_pressed();
    bool pressed(int keyMap) const;

private:
    KeyMap mapping;
    Uint32 eventType;
    SDL_Keycode keyCode;
    int map;
    Uint8 repeat;
    const Uint8 *keys;
    std::set<int> pressedKeyMaps;
};


#endif
