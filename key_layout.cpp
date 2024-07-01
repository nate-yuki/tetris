#include "key_layout.hpp"
#include "game.hpp"
#include "logger.hpp"


void KeyLayout::init (KeyMap &mapping, GamepadManager *gamepads, int gamepadInd)
{
    this->mapping = mapping;
    this->gamepads = gamepads;
    this->gamepadInd = gamepadInd;
}

void KeyLayout::handle_event (Game &game, const SDL_Event &e)
{
    eventType = e.type;
    map = -1;
    code = -1;
    repeat = -1;
    
    switch (eventType)
    {
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
        code = e.jbutton.button + GP_CODE_SEP;
        repeat = 0;
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        code = e.key.keysym.sym;
        repeat = e.key.repeat;
        break;
    }

    switch (eventType)
    {
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
        if (
            gamepads->get_id(gamepadInd) != e.jbutton.which &&
            gamepadInd != GAMEPAD_ANY
        )
        {
            break;
        }
        // Falls through
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        for (const auto &mapKeys : mapping)
        {
            if (mapKeys.second.find(code) != mapKeys.second.end())
            {
                map = mapKeys.first;
                break;
            }
        }
        break;
    }
}

KeyLayout::EventType KeyLayout::get_type () const
{
    switch (eventType)
    {
    case SDL_KEYDOWN:
    case SDL_JOYBUTTONDOWN:
        return DOWN;
    case SDL_KEYUP:
    case SDL_JOYBUTTONUP:
        return UP;
    }
    return NONE;
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
            if (key >= GP_CODE_SEP)
            {
                if (gamepads->button_pressed(gamepadInd, key - GP_CODE_SEP))
                {
                    pressedKeyMaps.insert(mapKeys.first);
                    break;
                }
            }
        }
    }
}

bool KeyLayout::pressed (int keyMap) const
{
    return pressedKeyMaps.find(keyMap) != pressedKeyMaps.end();
}
