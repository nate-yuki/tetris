/**
 * @file  util.hpp
 * @brief File header for various utility funcitons and structures.
 */

#ifndef UTIL_HPP
#define UTIL_HPP


#include <SDL2/SDL.h>

#include <vector>


using Scheme = std::vector<std::vector<int>>;
using SchemeRow = std::vector<int>;
using SchemeElem = int;


/// Struct containing RGB color components in SDL format.
struct Color
{
    Uint8 r, g, b;
};


/// Check collision between AABB rectangles.
bool check_collision_aabb(const SDL_Rect &a, const SDL_Rect &b);

/// Find minimum of `x` and `y`.
int min(int x, int y);

/// Find maximum of `x` and `y`.
int max(int x, int y);


#endif
