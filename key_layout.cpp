#include "key_layout.hpp"

KeyLayout::KeyLayout (KeyMap &&mapping)
    : mapping(mapping)
{}

void KeyLayout::handle_event (Game &game, const SDL_Event &e)
{
    eventType = e.type;
    map = -1;
    switch (eventType)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        keyCode = e.key.keysym.sym;
        repeat = e.key.repeat;
        for (const auto &mapKeys : mapping)
        {
            if (mapKeys.second.find(keyCode) != mapKeys.second.end())
            {
                map = mapKeys.first;
                break;
            }
        }
        break;
    }
}

Uint32 KeyLayout::get_type () const
{
    return eventType;
}

int KeyLayout::get_map () const
{
    return map;
}

Uint8 KeyLayout::get_repeat () const
{
    return repeat;
}

void KeyLayout::store_pressed ()
{
    keys = SDL_GetKeyboardState(NULL);
    pressedKeyMaps.clear();
    for (const auto &mapKeys : mapping)
    {
        for (const auto &key : mapKeys.second)
        {
            if (keys[SDL_GetScancodeFromKey(key)])
            {
                pressedKeyMaps.insert(mapKeys.first);
                break;
            }
        }
    }
}

bool KeyLayout::pressed (int keyMap) const
{
    return pressedKeyMaps.find(keyMap) != pressedKeyMaps.end();
}
