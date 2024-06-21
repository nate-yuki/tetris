/**
 * @file  main.cpp
 * @brief Main file.
 */

#include "game.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <cstdio>


int main (int argc, char *argv[])
{
    Game game;
    int exitCode = 0;

    try
    {
        Logger::get()->init("log.txt");
        game.init();

        // Game loop
        while (!game.is_over())
        {
            game.handle_events();
            game.do_logic();
            game.change_state();
            game.render();
        }
    }
    catch (const Exception &e)
    {
        printf("%s\n", e.what().c_str());
        exitCode = e.get_exit_code();
    }
    catch (std::exception &e)
    {
        printf("Standard exception: %s\n", e.what());
        exitCode = -1;
    }

    game.free();
    Logger::get()->flush();
    Logger::get()->free();
    
    return exitCode;
}
