#include "audio.hpp"
#include "exceptions.hpp"


std::map<int, const std::string> Audio::musicPath{
    {TITLE, "music/title.wav"},
    {TETRIS, "music/tetris.wav"},
};

std::map<int, const std::string> Audio::soundPath{
    {UNMUTE, "sounds/unmute.wav"},
    {GAME_START, "sounds/game_start.wav"},
    {GAME_PAUSE, "sounds/game_pause.wav"},
    {GAME_UNPAUSE, "sounds/game_unpause.wav"},
    {MENU_SCROLL, "sounds/menu_scroll.wav"},
    {MENU_SELECT, "sounds/menu_select.wav"},
    {TETRIMINO_FALL, "sounds/tetrimino_fall.wav"},
    {TETRIMINO_MOVE, "sounds/tetrimino_move.wav"},
    {TETRIMINO_DROP, "sounds/tetrimino_drop.wav"},
    {TETRIMINO_ROTATE, "sounds/tetrimino_rotate.wav"},
    {TETRIMINO_STOP, "sounds/tetrimino_stop.wav"},
    {TETRIMINO_BLOCKED, "sounds/tetrimino_blocked.wav"},
    {TETRIS_SINGLE, "sounds/tetris_single.wav"},
    {TETRIS_DOUBLE, "sounds/tetris_double.wav"},
    {TETRIS_TRIPLE, "sounds/tetris_triple.wav"},
    {TETRIS_TETRIS, "sounds/tetris_tetris.wav"},
    {TETRIS_SWAP, "sounds/tetris_swap.wav"},
    {TETRIS_GAME_OVER, "sounds/tetris_game_over.wav"},
};

std::vector<Mix_Music *> Audio::music(MUSIC_TOTAL);
std::vector<Mix_Chunk *> Audio::sounds(SOUNDS_TOTAL);

bool Audio::muted = false;
int Audio::lastVolSound = VOL_SOUND;
int Audio::lastVolMusic = VOL_MUSIC;
Audio::Music Audio::currMusic = Audio::MUSIC_TOTAL;


void Audio::init ()
{
    music.resize(MUSIC_TOTAL);
    sounds.resize(SOUNDS_TOTAL);

    for (int i = 0; i < MUSIC_TOTAL; ++i)
    {
        music[i] = Mix_LoadMUS(musicPath[i].c_str());
        if (music[i] == NULL)
        {
            throw ExceptionSDL(__FILE__, __LINE__, Mix_GetError());
        }
    }
    for (int i = 0; i < SOUNDS_TOTAL; ++i)
    {
        sounds[i] = Mix_LoadWAV(soundPath[i].c_str());
        if (sounds[i] == NULL)
        {
            throw ExceptionSDL(__FILE__, __LINE__, Mix_GetError());
        }
    }

    Mix_MasterVolume(VOL_SOUND);
    Mix_VolumeMusic(VOL_MUSIC);
}

void Audio::free ()
{
    for (int i = 0; i < MUSIC_TOTAL; ++i)
    {
        Mix_FreeMusic(music[i]);
    }
    for (int i = 0; i < SOUNDS_TOTAL; ++i)
    {
        Mix_FreeChunk(sounds[i]);
    }
    music.resize(MUSIC_TOTAL);
    sounds.resize(SOUNDS_TOTAL);
}

void Audio::mute ()
{
    lastVolSound = Mix_MasterVolume(0);
    lastVolMusic = Mix_VolumeMusic(0);
    muted = true;
}

void Audio::unmute ()
{
    Mix_MasterVolume(lastVolSound);
    Mix_VolumeMusic(lastVolMusic);
    muted = false;
}

bool Audio::toggle_sound ()
{
    if (muted)
    {
        unmute();
    }
    else
    {
        mute();
    }
    return !muted;
}

void Audio::set_music (Music music)
{
    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
    }
    Mix_PlayMusic(Audio::music[music], -1);
    currMusic = music;
}

void Audio::play_music (Music music)
{
    if (currMusic != music)
    {
        set_music(music);
    }
}

void Audio::stop_music (Music music)
{
    if (music == MUSIC_TOTAL || music == currMusic)
    {
        Mix_HaltMusic();
    }
}

void Audio::pause_music ()
{
    Mix_PauseMusic();
}

void Audio::unpause_music ()
{
    Mix_ResumeMusic();
}

void Audio::play_sound (Sound sound)
{
    Mix_PlayChannel(-1, sounds[sound], 0);
}
