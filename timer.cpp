/**
 * @file  timer.cpp
 * @brief Implementation of the class Timer.
 */

#include "timer.hpp"

#include <SDL2/SDL.h>


Timer::Timer (): startTime(SDL_GetTicks()), pauseTime(0) {}

void Timer::start ()
{
    startTime = SDL_GetTicks();
    pauseTime = 0;
}

Uint32 Timer::get_elapsed () const
{
    if (pauseTime)
    {
        return pauseTime - startTime;
    }
    return SDL_GetTicks() - startTime;
}

void Timer::pause ()
{
    if (!pauseTime)
    {
        pauseTime = SDL_GetTicks();
    }
}

void Timer::unpause ()
{
    if (pauseTime)
    {
        // Substract time between pause and previous start so that it will be added
        // on elapsed time calculation
        startTime = SDL_GetTicks() - (pauseTime - startTime);
        pauseTime = 0;
    }
}
