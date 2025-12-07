/**
 * @file  timer.hpp
 * @brief Include file for Timer class.
 */

#ifndef TIMER_HPP
#define TIMER_HPP


#include <SDL2/SDL.h>


/**
 * @brief An SDL based timer.
 * @example
 * 
 *     Timer timer;
 *     timer.start(); // Can be ommitted
 *     std::cout << timer.get_elpased() << std::endl; // Time since start
 *     timer.pause();
 *     std::cout << timer.get_elpased() << std::endl; // Time between start and pause
 *     timer.unpause();
 *     std::cout << timer.get_elpased() << std::endl; // Time between start and pause
 *                                                    // + time since unpause
 *     timer.start(); // Start from 0
 *     std::cout << timer.get_elpased() << std::endl; // Time since new start
 *     timer.pause();
 *     timer.start(); // Unpause and start from 0
 *     std::cout << timer.get_elpased() << std::endl;
 */
class Timer
{
public:
    /// Initialize start time.
    Timer();

    /// Unpause and set new start time.
    void start();

    /// Pause the timer if not paused.
    void pause();

    /// Unpause the timer if paused.
    void unpause();

    /// Get total running time from the last start time setting.
    Uint32 get_elapsed() const;

private:
    Uint32 startTime; // Time on start.
    Uint32 pauseTime; // Time on pause.
};


#endif
