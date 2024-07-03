#ifndef AUDIO_HPP
#define AUDIO_HPP


#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>
#include <map>


/// A purely static class implementing audio interface.
class Audio
{
public:
    /// Music indeces.
    enum Music{
        TITLE,
        TETRIS,
        MUSIC_TOTAL,
    };

    /// Sound indeces.
    enum Sound{
        UNMUTE,
        GAME_START,
        GAME_PAUSE,
        GAME_UNPAUSE,
        MENU_SCROLL,
        MENU_SELECT,
        TETRIMINO_FALL,
        TETRIMINO_MOVE,
        TETRIMINO_DROP,
        TETRIMINO_ROTATE,
        TETRIMINO_STOP,
        TETRIMINO_BLOCKED,
        TETRIS_SINGLE,
        TETRIS_DOUBLE,
        TETRIS_TRIPLE,
        TETRIS_TETRIS,
        TETRIS_SWAP,
        TETRIS_GAME_OVER,
        SOUNDS_TOTAL,
    };

    static constexpr int VOL_SOUND = MIX_MAX_VOLUME / 32; // Sound volume
    static constexpr int VOL_MUSIC = VOL_SOUND / 2; // Music volume

    /// Load all music and sounds.
    static void init();

    /// Free the loaded music and sounds.
    static void free();

    /// Mute both the sounds and the currently playing music.
    static void mute();

    /// Unmute both the sounds and the currently playing music.
    static void unmute();

    /**
     * @brief Toggle between muted/unmuted for both the sounds and the currently
     *     playing music.
     * @return `true` if the volume was unmuted.
     */
    static bool toggle_sound();

    /// Stops the currently playing music and plays `music`.
    static void set_music(Music music);

    /// If the currently playing music is not `music`, plays `music`.
    static void play_music(Music music);

    /**
     * @brief Stops the currently playing music.
     * @param music If specified, only stops music if the current music is `music`.
     */
    static void stop_music(Music music=MUSIC_TOTAL);

    /// Pauses the currently playing music.
    static void pause_music();

    /// Unpauses the currently playing music.
    static void unpause_music();

    /// Plays `sound` a single time on the first free channel.
    static void play_sound(Sound sound);

private:
    /// (Music, path to file with Music) pairs.
    static std::map<int, const std::string> musicPath;

    /// (Sound, path to file with Sound) pairs.
    static std::map<int, const std::string> soundPath;

    static std::vector<Mix_Music *> music; // The loaded music.
    static std::vector<Mix_Chunk *> sounds; // The loaded sounds.

    static bool muted;
    static int lastVolSound; // Sound volume before muting.
    static int lastVolMusic; // Music volume before muting.

    static Music currMusic;
};


#endif
