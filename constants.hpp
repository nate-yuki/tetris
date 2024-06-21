/**
 * @file  constants.hpp
 * @brief Header file with constants definitions.
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


/// If true, flush all log messages.
#ifdef DEBUG
constexpr bool FLUSH_ALL = true;
#else
constexpr bool FLUSH_ALL = false;
#endif

/// Window dimensions on start.
constexpr int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;

/// Maximum font point size.
constexpr int MAX_PT_SIZE = 150;

/// Maximum particle size.
constexpr int MAX_PARTICLE_SIZE = 50;

/// Maximum block size.
constexpr int MAX_BLOCK_SIZE = 100;

/// Maximum scheme length.
constexpr int MAX_SCHEME_LEN = 4;

/// Tetris field dimentions.
constexpr int TETRIS_FIELD_WIDTH = 10, TETRIS_FIELD_HIGHT = 20;

/// Time to keep rendering cleared lines for.
constexpr int CLEAR_LINE_RENDER_TIME = 150;


#endif
