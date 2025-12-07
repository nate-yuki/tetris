/**
 * @file  util.hpp
 * @brief File header for various utility funcitons and structures.
 */

#ifndef UTIL_HPP
#define UTIL_HPP


#include <SDL2/SDL.h>
#include <vector>
#include <string>


/// A struct containing RGB color components in SDL format.
struct Color
{
    Uint8 r, g, b;
};


constexpr Color BLACK {0, 0, 0};
constexpr Color RED {255, 0, 0};
constexpr Color GREEN {0, 255, 0};
constexpr Color BLUE {0, 0, 255};
constexpr Color YELLOW {255, 255, 0};
constexpr Color MAGENTA {255, 0, 255};
constexpr Color CYAN {0, 255, 255};
constexpr Color WHITE {255, 255, 255};


/// Check collision between AABB rectangles.
bool check_collision_aabb(const SDL_Rect &a, const SDL_Rect &b);

/// Find minimum of `x` and `y`.
int min(int x, int y);

/// Find maximum of `x` and `y`.
int max(int x, int y);

/**
 * @brief Get a `str` with `padding` added to the beginning for a total length `len`.
 * @note If `str` length is greater than `len`, produces an error.
 * @param str The string to pad.
 * @param len The total length of the resulting string.
 * @param padding Character to pad with.
 * @return The padded string.
 */
std::string get_padded(const std::string &str, int len, char padding);


#endif
