/**
 * @file  states.cpp
 * @brief Implementation of the game state classes.
 */

#include "states.hpp"
#include "audio.hpp"
#include "util.hpp"
#include "constants.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <fstream>


// Singleton instances

TitleScreenState TitleScreenState::sTitleScreenState;
MenuState MenuState::sMenuState;
PlayersSelectState PlayersSelectState::sPlayersSelectState;
TetrisState TetrisState::sTetrisState;
TetrisPVPState TetrisPVPState::sTetrisPVPState;
ResultsScreenState ResultsScreenState::sResultsScreenState;
GameOverState GameOverState::sGameOverState;

KeyMap TitleScreenState::keyMap{
    {
        TitleScreenState::Commands::START,
        {SDLK_RETURN, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A}
    },
};

KeyMap TetrisState::keyMap{
    {
        TetrisState::Commands::END,
        {SDLK_END, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_RIGHTSTICK}
    },
};
KeyMap TetrisState::tetrisKeyMap{
    {
        TetrisLayout::Commands::SWAP,
        {
            SDLK_SPACE,
            KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
            KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
        }
    },
};
KeyMap TetrisState::tetriminoKeyMap{
    {
        Tetrimino::Commands::RIGHT,
        {
            SDLK_RIGHT, SDLK_d,
            KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_RIGHT
        }
    },
    {
        Tetrimino::Commands::LEFT,
        {SDLK_LEFT, SDLK_a, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_LEFT}
    },
    {
        Tetrimino::Commands::ACC,
        {SDLK_DOWN, SDLK_s, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_DOWN}
    },
    {
        Tetrimino::Commands::DROP,
        {SDLK_UP, SDLK_w, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_UP}
    },
    {
        Tetrimino::Commands::ROT_CCW,
        {
            SDLK_RSHIFT, SDLK_q,
            KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_B,
            KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_Y
        }
    },
    {
        Tetrimino::Commands::ROT_CW,
        {
            SDLK_e, SDLK_KP_1,
            KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A,
            KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_X
        }
    },
};

KeyMap TetrisPVPState::keyMap{
    {
        TetrisPVPState::Commands::END,
        {SDLK_END, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_RIGHTSTICK}
    },
};
std::vector<KeyMap> TetrisPVPState::tetrisKeyMaps{
    {
        {
            TetrisLayout::Commands::SWAP,
            {
                SDLK_SPACE,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
            }
        }
    },
    {
        {
            TetrisLayout::Commands::SWAP,
            {
                SDLK_RETURN,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
            }
        }
    },
    {
        {
            TetrisLayout::Commands::SWAP,
            {
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
            }
        }
    },
    {
        {
            TetrisLayout::Commands::SWAP,
            {
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
            }
        }
    },
};
std::vector<KeyMap> TetrisPVPState::tetriminoKeyMaps{
    {
        {
            Tetrimino::Commands::RIGHT,
            {SDLK_d, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
        },
        {
            Tetrimino::Commands::LEFT,
            {SDLK_a, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_LEFT}
        },
        {
            Tetrimino::Commands::ACC,
            {SDLK_s, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_DOWN}
        },
        {
            Tetrimino::Commands::DROP,
            {SDLK_w, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_UP}
        },
        {
            Tetrimino::Commands::ROT_CCW,
            {
                SDLK_q,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_B,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_Y
            }
        },
        {
            Tetrimino::Commands::ROT_CW,
            {
                SDLK_e,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_X
            }
        },
    },
    {
        {
            Tetrimino::Commands::RIGHT,
            {SDLK_RIGHT, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
        },
        {
            Tetrimino::Commands::LEFT,
            {SDLK_LEFT, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_LEFT}
        },
        {
            Tetrimino::Commands::ACC,
            {SDLK_DOWN, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_DOWN}
        },
        {
            Tetrimino::Commands::DROP,
            {SDLK_UP, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_UP}
        },
        {
            Tetrimino::Commands::ROT_CCW,
            {
                SDLK_RSHIFT,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_B,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_Y
            }
        },
        {
            Tetrimino::Commands::ROT_CW,
            {
                SDLK_KP_1,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_X
            }
        },
    },
    {
        {
            Tetrimino::Commands::RIGHT,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
        },
        {
            Tetrimino::Commands::LEFT,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_LEFT}
        },
        {
            Tetrimino::Commands::ACC,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_DOWN}
        },
        {
            Tetrimino::Commands::DROP,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_UP}
        },
        {
            Tetrimino::Commands::ROT_CCW,
            {
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_B,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_Y
            }
        },
        {
            Tetrimino::Commands::ROT_CW,
            {
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_X
            }
        },
    },
    {
        {
            Tetrimino::Commands::RIGHT,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_RIGHT}
        },
        {
            Tetrimino::Commands::LEFT,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_LEFT}
        },
        {
            Tetrimino::Commands::ACC,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_DOWN}
        },
        {
            Tetrimino::Commands::DROP,
            {KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_DPAD_UP}
        },
        {
            Tetrimino::Commands::ROT_CCW,
            {
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_B,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_Y
            }
        },
        {
            Tetrimino::Commands::ROT_CW,
            {
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A,
                KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_X
            }
        },
    },
};

KeyMap ResultsScreenState::keyMap{
    {
        ResultsScreenState::Commands::SKIP,
        {SDLK_RETURN, KeyLayout::GP_CODE_SEP + SDL_CONTROLLER_BUTTON_A}
    },
};


TitleScreenState::TitleScreenState () {}

TitleScreenState *TitleScreenState::get ()
{
    return &sTitleScreenState;
}

void TitleScreenState::enter (Game *game)
{
    log("Entering TitleScreen", __FILE__, __LINE__);

    this->game = game;
    
    game->load_texture_from_file(bgTexture, "textures/bg.png");
    game->create_text(titleText, "TETRIS", WHITE);
    game->create_text(promptText, "(press Enter to start)", WHITE);

    game->create_key_loadout(
        keyLayout, keyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY
    );

    Audio::set_music(Audio::TITLE);
}

void TitleScreenState::exit ()
{
    log("Exiting TitleScreen", __FILE__, __LINE__);

    bgTexture.free();
    titleText.free();
    promptText.free();
}

void TitleScreenState::handle_event (Game &game, const SDL_Event &e)
{
    keyLayout.handle_event(game, e);
    if (keyLayout.get_type() == KeyLayout::DOWN && !game.is_paused())
    {
        switch (keyLayout.get_command())
        {
        case START:
            game.set_next_state(MenuState::get());
            Audio::play_sound(Audio::GAME_START);
            break;
        }
    }
}

void TitleScreenState::do_logic () {}

void TitleScreenState::render ()
{
    bgTexture.render(
        (game->get_renderer_width() - bgTexture.get_width()) / 2,
        (game->get_renderer_height() - bgTexture.get_height()) / 2
    );

    titleText.render(
        0, 0, game->get_renderer_width(), game->get_renderer_height() / 2,
        Text::TEXT_CENTER_BOTTOM
    );
    promptText.render(
        0, game->get_renderer_height() / 2,
        game->get_renderer_width(), game->get_renderer_height() / 2,
        Text::TEXT_CENTER_TOP
    );
}

void TitleScreenState::pause_timers () {}
void TitleScreenState::unpause_timers () {}


MenuState::MenuState () {}

MenuState *MenuState::get ()
{
    return &sMenuState;
}

void MenuState::enter (Game *game)
{
    log("Entering Menu", __FILE__, __LINE__);

    this->game = game;

    game->create_menu(
        menu, "Choose an option:", {"Singleplayer", "Multiplayer", "Exit"}
    );

    Audio::play_music(Audio::TITLE);
}

void MenuState::exit ()
{
    log("Exiting Menu", __FILE__, __LINE__);

    menu.free();
}

void MenuState::handle_event (Game &game, const SDL_Event &e)
{
    if (!game.is_paused())
    {
        menu.handle_event(game, e);
    }
}

void MenuState::do_logic ()
{
    if (menu.choosen_option() != -1)
    {
        switch (menu.choosen_option())
        {
        case 0:
            game->set_next_state(TetrisState::get());
            break;
        case 1:
            game->set_next_state(PlayersSelectState::get());
            break;
        case 2:
            game->set_next_state(GameOverState::get());
            break;
        }
    }
}

void MenuState::render ()
{
    menu.render(0, 0, game->get_renderer_width(), game->get_renderer_height());
}

void MenuState::pause_timers () {}
void MenuState::unpause_timers () {}


PlayersSelectState::PlayersSelectState () {}

PlayersSelectState *PlayersSelectState::get()
{
    return &sPlayersSelectState;
}

void PlayersSelectState::enter (Game *game)
{
    log("Entering PlayersSelect", __FILE__, __LINE__);

    this->game = game;

    game->create_menu(
        menu, "Select the amount of players:", {"2", "3", "4", "Back"}
    );

    Audio::play_music(Audio::TITLE);
}

void PlayersSelectState::exit ()
{
    log("Exiting PlayersSelect", __FILE__, __LINE__);

    menu.free();
}

void PlayersSelectState::handle_event (Game &game, const SDL_Event &e)
{
    if (!game.is_paused())
    {
        menu.handle_event(game, e);
    }
}

void PlayersSelectState::do_logic ()
{
    if (menu.choosen_option() != -1)
    {
        switch (menu.choosen_option())
        {
        case 0:
        case 1:
        case 2:
            game->set_players(2 + menu.choosen_option());
            game->set_next_state(TetrisPVPState::get());
            break;
        case 3:
            game->set_next_state(MenuState::get());
            break;
        }
    }
}

void PlayersSelectState::render ()
{
    menu.render(0, 0, game->get_renderer_width(), game->get_renderer_height());
}

void PlayersSelectState::pause_timers () {}
void PlayersSelectState::unpause_timers () {}


TetrisState::TetrisState () {}

TetrisState *TetrisState::get ()
{
    return &sTetrisState;
}

void TetrisState::enter (Game *game)
{
    log("Entering Tetris", __FILE__, __LINE__);

    this->game = game;

    std::ifstream highScoreFile(
        "high_score.hs", std::ifstream::in | std::ifstream::binary
    );
    if (highScoreFile.fail())
    {
        std::string msg = "Could not open \"" "high_score.hs" "\"";
        throw ExceptionFile(__FILE__, __LINE__, msg.c_str());
    }
    highScoreFile >> highScore;
    if (highScoreFile.fail())
    {
        std::string msg = "Could not read from \"" "high_score.hs" "\"";
        throw ExceptionFile(__FILE__, __LINE__, msg.c_str());
    }
    highScoreFile.close();
    
    game->load_texture_from_file(bgTexture, "textures/bg.png");
    game->load_texture_from_file(blockTextureSheet, "textures/blocks.png", &CYAN);
    game->load_texture_from_file(fieldBgTexture, "textures/field_bg.png");
    game->load_texture_from_file(fieldFrameTexture, "textures/field_frame.png");
    game->load_texture_from_file(fieldClearTexture, "textures/field_clear.png");
    game->load_texture_from_file(
        fieldClearParticleTextureSheet, "textures/field_particles.png", &CYAN
    );

    game->create_text(linesClearedText, "0000", WHITE, "999999999");
    game->create_text(linesClearedPromptText, "Lines cleared:", WHITE);
    game->create_text(scoreText, "000000000", WHITE, "999999999");
    game->create_text(scorePromptText, "Score:", WHITE, "High score:");
    game->create_text(
        highScoreText,
        get_padded(std::to_string(highScore), 9, '0'), WHITE, "999999999"
    );
    game->create_text(highScorePromptText, "High score:", WHITE);
    game->create_text(msgText, "", WHITE, std::string(24, 'W'));
    game->create_text(comboText, "Combo: 0", WHITE, "Combo: 99");

    game->create_key_loadout(
        keyLayout, keyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY
    );
    game->create_key_loadout(
        tetrisLayout, tetrisKeyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY
    );
    game->create_key_loadout(
        tetriminoLayout, tetriminoKeyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY
    );

    tetris.init(
        TETRIS_FIELD_WIDTH, TETRIS_FIELD_HEIGHT,
        &tetrisLayout, &tetriminoLayout,
        &tetriminoTimer, &clearLineTimer, &gameOverTimer,
        &msgTextTimer,
        &bgTexture, &blockTextureSheet,
        &fieldBgTexture, &fieldFrameTexture, &fieldClearTexture,
        &fieldClearParticleTextureSheet,
        &linesClearedText, &linesClearedPromptText,
        &scoreText, &scorePromptText,
        &highScoreText, &highScorePromptText,
        &msgText, &comboText
    );
    tetriminoTimer.start();

    Audio::set_music(Audio::TETRIS);
}

void TetrisState::exit ()
{
    log("Exiting Tetris", __FILE__, __LINE__);

    int score = tetris.get_score();
    game->set_scores(score, highScore);
    game->set_players(1);

    if (score > highScore)
    {
        std::ofstream highScoreFile(
            "high_score.hs", std::ofstream::out | std::ofstream::binary
        );
        if (highScoreFile.fail())
        {
            std::string msg = "Could not open \"" "high_score.hs" "\"";
            throw ExceptionFile(__FILE__, __LINE__, msg.c_str());
        }
        highScoreFile << score;
        if (highScoreFile.fail())
        {
            std::string msg = "Could not write to \"" "high_score.hs" "\"";
            throw ExceptionFile(__FILE__, __LINE__, msg.c_str());
        }
        highScoreFile.close();
    }

    bgTexture.free();
    blockTextureSheet.free();
    fieldBgTexture.free();
    fieldFrameTexture.free();
    fieldClearTexture.free();
    fieldClearParticleTextureSheet.free();

    linesClearedText.free();
    linesClearedPromptText.free();
    scoreText.free();
    scorePromptText.free();
    highScoreText.free();
    highScorePromptText.free();
    msgText.free();
    comboText.free();

    tetris.free();

    Audio::stop_music(Audio::TETRIS);
}

void TetrisState::handle_event (Game &game, const SDL_Event &e)
{
    // Force transition to ResultsScreenState
    keyLayout.handle_event(game, e);
    if (keyLayout.get_type() == KeyLayout::DOWN && !game.is_paused())
    {
        switch (keyLayout.get_command())
        {
        case END:
            game.set_next_state(ResultsScreenState::get());
            break;
        }
    }

    tetris.handle_event(game, e);
}

void TetrisState::do_logic ()
{
    if (tetris.game_over())
    {
        if (gameOverTimer.get_elapsed() >= 1500)
        {
            game->set_next_state(ResultsScreenState::get());
        }
        
        Audio::stop_music(Audio::TETRIS);
    }
    else
    {
        tetris.do_logic();
    }
}

void TetrisState::render ()
{
    tetris.render(0, 0, game->get_renderer_width(), game->get_renderer_height());
}

void TetrisState::pause_timers ()
{
    tetriminoTimer.pause();
    clearLineTimer.pause();
    gameOverTimer.pause();
    msgTextTimer.pause();
}

void TetrisState::unpause_timers ()
{
    tetriminoTimer.unpause();
    clearLineTimer.unpause();
    gameOverTimer.unpause();
    msgTextTimer.unpause();
}


TetrisPVPState::TetrisPVPState () {}

TetrisPVPState *TetrisPVPState::get ()
{
    return &sTetrisPVPState;
}

void TetrisPVPState::enter (Game *game)
{
    log("Entering TetrisPVP", __FILE__, __LINE__);

    this->game = game;
    
    // Initializing shared objects
    game->load_texture_from_file(bgTexture, "textures/bg.png");
    game->load_texture_from_file(blockTextureSheet, "textures/blocks.png", &CYAN);
    game->load_texture_from_file(fieldBgTexture, "textures/field_bg.png");
    game->load_texture_from_file(fieldFrameTexture, "textures/field_frame.png");
    game->load_texture_from_file(fieldClearTexture, "textures/field_clear.png");
    game->load_texture_from_file(
        fieldClearParticleTextureSheet, "textures/field_particles.png", &CYAN
    );
    
    game->create_text(linesClearedPromptText, "Lines cleared:", WHITE, "999999999");
    game->create_text(scorePromptText, "Score:", WHITE, "999999999");

    game->create_key_loadout(
        keyLayout, keyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY
    );

    players = game->get_players();

    // Initializing individual objects
    linesClearedTexts.resize(players);
    scoreTexts.resize(players);
    msgTexts.resize(players);
    comboTexts.resize(players);
    tetris.resize(players);
    tetriminoTimers.resize(players);
    clearLineTimers.resize(players);
    msgTextTimers.resize(players);
    gameOverTimers.resize(players);
    tetrisLayouts.resize(players);
    tetriminoLayouts.resize(players);

    TetrisLayout::Layout layout;
    switch (players)
    {
    case 2:
    case 3:
        layout = TetrisLayout::REDUCED;
        break;
    case 4:
        layout = TetrisLayout::MINIMAL;
        break;
    }

    for (int i = 0; i < players; ++i)
    {
        game->create_text(linesClearedTexts[i], "0000", WHITE, "999999999");
        game->create_text(scoreTexts[i], "000000000", WHITE, "999999999");
        game->create_text(msgTexts[i], "", WHITE, std::string(24, 'W'));
        game->create_text(comboTexts[i], "Combo: 0", WHITE, "999999999");

        game->create_key_loadout(tetrisLayouts[i], tetrisKeyMaps[i], i);
        game->create_key_loadout(tetriminoLayouts[i], tetriminoKeyMaps[i], i);

        tetris[i].init(
            TETRIS_FIELD_WIDTH, TETRIS_FIELD_HEIGHT,
            &tetrisLayouts[i], &tetriminoLayouts[i],
            &tetriminoTimers[i], &clearLineTimers[i], &gameOverTimers[i],
            &msgTextTimers[i],
            &bgTexture, &blockTextureSheet,
            &fieldBgTexture, &fieldFrameTexture, &fieldClearTexture,
            &fieldClearParticleTextureSheet,
            &linesClearedTexts[i], &linesClearedPromptText,
            &scoreTexts[i], &scorePromptText,
            nullptr, nullptr,
            &msgTexts[i], &comboTexts[i],
            layout
        );
    }
    for (int i = 0; i < players; ++i)
    {
        tetriminoTimers[i].start();
    }

    Audio::set_music(Audio::TETRIS);
}

void TetrisPVPState::exit ()
{
    log("Exiting TetrisPVP", __FILE__, __LINE__);

    // Pass the best score player index and their score to game
    int score, highScore = -1, winner;
    for (int i = 0; i < players; ++i)
    {
        score = tetris[i].get_score();
        if (score > highScore)
        {
            highScore = score;
            winner = i;
        }
    }
    game->set_scores(highScore, highScore);
    game->set_winner(winner);

    bgTexture.free();
    blockTextureSheet.free();
    fieldBgTexture.free();
    fieldFrameTexture.free();
    fieldClearTexture.free();
    fieldClearParticleTextureSheet.free();

    linesClearedPromptText.free();
    scorePromptText.free();

    for (int i = 0; i < players; ++i)
    {
        linesClearedTexts[i].free();
        scoreTexts[i].free();
        msgTexts[i].free();
        comboTexts[i].free();

        tetris[i].free();
    }

    Audio::stop_music(Audio::TETRIS);
}

void TetrisPVPState::handle_event (Game &game, const SDL_Event &e)
{
    // Force transition to ResultsScreenState
    keyLayout.handle_event(game, e);
    if (keyLayout.get_type() == KeyLayout::DOWN && !game.is_paused())
    {
        switch (keyLayout.get_command())
        {
        case END:
            game.set_next_state(ResultsScreenState::get());
            break;
        }
    }

    for (int i = 0; i < players; ++i)
    {
        tetris[i].handle_event(game, e);
    }
}

void TetrisPVPState::do_logic ()
{
    int players_checked;
    for (players_checked = 0; players_checked < players; ++players_checked)
    {
        if (!tetris[players_checked].game_over())
        {
            break;
        }
    }
    // If all players got a game over, end the game
    if (players_checked == players)
    {
        int timers_checked;
        for (timers_checked = 0; timers_checked < players; ++timers_checked)
        {
            if (gameOverTimers[timers_checked].get_elapsed() < 1500)
            {
                break;
            }
        }
        // If all game over pauses have elapsed, transit to ResultsScreenState
        if (timers_checked == players)
        {
            game->set_next_state(ResultsScreenState::get());
        }

        Audio::stop_music(Audio::TETRIS);
    }
    else
    {
        // If at least one player hasn't reached game over, keep doing logic for them
        for (int i = 0; i < players; ++i)
        {
            if (!tetris[i].game_over())
            {
                tetris[i].do_logic();
            }
        }
    }
}

void TetrisPVPState::render ()
{
    switch (players)
    {
    case 2:
        tetris[0].render(
            0, 0,
            game->get_renderer_width() / 2, game->get_renderer_height()
        );
        tetris[1].render(
            game->get_renderer_width() / 2, 0,
            game->get_renderer_width() / 2, game->get_renderer_height()
        );
        break;
    case 3:
        tetris[0].render(
            0, 0,
            game->get_renderer_width() / 3, game->get_renderer_height()
        );
        tetris[1].render(
            game->get_renderer_width() / 3, 0,
            game->get_renderer_width() / 3, game->get_renderer_height()
        );

        // Rendering the background texture to fill empty space
        bgTexture.render(
            {
                game->get_renderer_width() / 3 * 2, 0,
                game->get_renderer_width() - game->get_renderer_width() / 3 * 2,
                game->get_renderer_height()
            }
        );
        // Chosing the values to compensate for the rounding error
        tetris[2].render(
            game->get_renderer_width() / 3 * 2, 0,
            game->get_renderer_width() / 3,
            //game->get_renderer_width() - game->get_renderer_width() / 3 * 2,
            game->get_renderer_height()
        );
        break;
    case 4:
        tetris[0].render(
            0, 0,
            game->get_renderer_width() / 2, game->get_renderer_height() / 2
        );
        tetris[1].render(
            game->get_renderer_width() / 2, 0,
            game->get_renderer_width() / 2, game->get_renderer_height() / 2
        );
        tetris[2].render(
            0, game->get_renderer_height() / 2,
            game->get_renderer_width() / 2, game->get_renderer_height() / 2
        );
        tetris[3].render(
            game->get_renderer_width() / 2, game->get_renderer_height() / 2,
            game->get_renderer_width() / 2, game->get_renderer_height() / 2
        );
        break;
    }
}

void TetrisPVPState::pause_timers ()
{
    for (int i = 0; i < players; ++i)
    {
        tetriminoTimers[i].pause();
        clearLineTimers[i].pause();
        gameOverTimers[i].pause();
        msgTextTimers[i].pause();
    }
}

void TetrisPVPState::unpause_timers ()
{
    for (int i = 0; i < players; ++i)
    {
        tetriminoTimers[i].unpause();
        clearLineTimers[i].unpause();
        gameOverTimers[i].unpause();
        msgTextTimers[i].unpause();
    }
}


ResultsScreenState::ResultsScreenState () {}

ResultsScreenState *ResultsScreenState::get ()
{
    return &sResultsScreenState;
}

void ResultsScreenState::enter (Game *game)
{
    log("Entering ResultsScreen", __FILE__, __LINE__);

    this->game = game;

    game->load_texture_from_file(bgTexture, "textures/bg.png");
    game->create_text(titleText, "Game over!", WHITE);

    game->create_key_loadout(
        keyLayout, keyMap, KeyLayout::GamepadSelector::GAMEPAD_ANY
    );

    std::string resultsMsg;
    int score = game->get_score();
    if (game->get_players() == 1)
    {
        int highScore = game->get_high_score();
        resultsMsg = "Score: " + std::to_string(score)
            + " (previous high score: " + std::to_string(highScore) + ")";
        if (score > highScore)
        {
            resultsMsg += " New high score!";
        }
    }
    else
    {
        resultsMsg = "Player " + std::to_string(game->get_winner() + 1)
            + " won with score " + std::to_string(score) + "!";
    }
    game->create_text(resultsText, resultsMsg, WHITE);

    resultsTimer.start();

    Audio::play_music(Audio::TITLE);
}

void ResultsScreenState::exit()
{
    log("Entering ResultsScreen", __FILE__, __LINE__);

    bgTexture.free();
    titleText.free();
    resultsText.free();
}

void ResultsScreenState::handle_event (Game &game, const SDL_Event &e)
{
    // Force transition to MenuState
    keyLayout.handle_event(game, e);
    if (keyLayout.get_type() == KeyLayout::DOWN && !game.is_paused())
    {
        switch (keyLayout.get_command())
        {
        case SKIP:
            if (game.get_players() == 1)
            {
                game.set_next_state(MenuState::get());
            }
            else
            {
                game.set_next_state(PlayersSelectState::get());
            }
            break;
        }
    }
}

void ResultsScreenState::do_logic ()
{
    // Wait before transitioning back to MenuState
    if (resultsTimer.get_elapsed() >= 5000)
    {
        if (game->get_players() == 1)
        {
            game->set_next_state(MenuState::get());
        }
        else
        {
            game->set_next_state(PlayersSelectState::get());
        }
    }
}

void ResultsScreenState::render ()
{
    bgTexture.render(
        (game->get_renderer_width() - bgTexture.get_width()) / 2,
        (game->get_renderer_height() - bgTexture.get_height()) / 2
    );

    titleText.render(
        0, 0, game->get_renderer_width(), game->get_renderer_height() / 2,
        Text::TEXT_CENTER_BOTTOM
    );
    resultsText.render(
        0, game->get_renderer_height() / 2,
        game->get_renderer_width(), game->get_renderer_height() / 2,
        Text::TEXT_CENTER_TOP
    );
}

void ResultsScreenState::pause_timers ()
{
    resultsTimer.pause();
}

void ResultsScreenState::unpause_timers ()
{
    resultsTimer.unpause();
}

GameOverState::GameOverState () {}

GameOverState *GameOverState::get ()
{
    return &sGameOverState;
}

void GameOverState::enter (Game *game)
{
    log("Entering GameOver", __FILE__, __LINE__);
    
    Audio::stop_music();
}

void GameOverState::exit ()
{
    log("Exiting GameOver", __FILE__, __LINE__);
}

void GameOverState::handle_event (Game &game, const SDL_Event &e) {}
void GameOverState::do_logic () {}
void GameOverState::render () {}
void GameOverState::pause_timers () {}
void GameOverState::unpause_timers () {}
