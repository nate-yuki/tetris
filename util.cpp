/**
 * @file  util.cpp
 * @brief File with implementation of various utility funcitons and structures.
 */

#include "util.hpp"


#include <SDL2/SDL.h>


bool check_collision_aabb (const SDL_Rect &a, const SDL_Rect &b)
{
    if (a.x + a.w <= b.x)
    {
        return false;
    }
    if (b.x + b.w <= a.x)
    {
        return false;
    }
    if (a.y + a.h <= b.y)
    {
        return false;
    }
    if (b.y + b.h <= a.y)
    {
        return false;
    }
    
    return true;
}

int min (int x, int y)
{
    return x <= y ? x : y;
}

int max (int x, int y)
{
    return x >= y ? x : y;
}

std::string get_padded (const std::string &str, int len, char padding)
{
    return std::string(len - str.size(), padding) + str;
}
