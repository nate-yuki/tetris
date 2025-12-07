/**
 * @file  tetrimino.hpp
 * @brief Include file for Tetrimino class and TetriminoConfig struct.
 */

#ifndef TETRIMINO_HPP
#define TETRIMINO_HPP


#include "tetris_field.hpp"
#include "texture.hpp"
#include "util.hpp"
#include "key_layout.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>


using SchemeElem = int;
using SchemeRow = std::vector<SchemeElem>;
using Scheme = std::vector<SchemeRow>;


struct TetriminoConfig;

class Block;
class TetrisField;
class Game;
class KeyLayout;

/// The tetrimino class.
class Tetrimino
{
public:
    /// Tetrimino commands.
    enum Commands{
        RIGHT, // Move the tetrimino to the right.
        LEFT, // Move the tetrimino to the left.
        ACC, // Increase the tetrimino falling speed.
        DROP, // Drop the tetrimino.
        ROT_CCW, // Rotate the tetrimino counter-clockwise.
        ROT_CW, // Rotate the tetrimino clockwise.
    };
    
    /// Tetrimino scheme type values.
    enum TetriminoType {
        TETRIMINO_I,
        TETRIMINO_T,
        TETRIMINO_L,
        TETRIMINO_LR,
        TETRIMINO_Z,
        TETRIMINO_ZR,
        TETRIMINO_O,
        TETRIMINO_TOTAL,
    };

    /// Tetrimino counter-clockwise rotation values.
    enum TetriminoRotation {
        TETRIMINO_ROTATION_0,
        TETRIMINO_ROTATION_90,
        TETRIMINO_ROTATION_180,
        TETRIMINO_ROTATION_270,
        TETRIMINO_ROTATION_TOTAL,
    };

    /**
     * @brief Read and store tetrimino schemes from `path`.
     * @throws `ExceptionFile` thrown if `path` does not exist or is not readable, or
     *     if there was an error during reading.
     */
    static void load_schemes(const std::string &path);
    
    /// Create clips to use for selecting a block texture from the sheet.
    static void init_clips();

    /**
     * @brief Render a specified tetrimino with given parameters without creating a
     *     tetrimino object.
     * @param config The tetrimino to render.
     * @param x Scheme upper left corner x coordinate.
     * @param y Scheme upper left corner y coordinate.
     * @param size The block size.
     * @param blockTextureSheet The texture sheet: last entry is the ghost tetrimino
     *     texture and all other are for different block textures.
     */
    static void render_config(
        const TetriminoConfig &config, int x, int y, int size,
        Texture *blockTextureSheet
    );

    /// Store `field`, `blockTextureSheet`, and `keyLayout`.
    void init(TetrisField *field, Texture *blockTextureSheet, KeyLayout *keyLayout);

    /**
     * @brief Free the blocks.
     * @param logMsg If `true`, log a message; default is `true`.
     */
    void free(bool logMsg=true);

    /**
     * @brief Check if the tetrimino fits, create blocks and initialize class members.
     * @param posX Field position x coordinate.
     * @param posY Field position y coordinate.
     * @param fallDelay Falling period.
     * @param config The tetrimino to spawn.
     * @param ghost If `true`, use the ghost texture; default is `false`.
     * @return `true` if the tetrimino fits into the field.
     */
    bool spawn(
        int posX, int posY, int fallDelay, const TetriminoConfig &config,
        bool ghost=false
    );

    /**
     * @brief If initialized, render the tetrimino with given parameters.
     * @details
     * If the tetrimino doesn't have blocks directly beneath, renders a ghost.
     * @note Actual coordinates are calculated based on the tetrimino field position.
     * @param x Field x coordinate.
     * @param y Field y coordinate.
     * @param size The block size.
     */
    void render(int x, int y, int size);

    /**
     * @brief If initialized, handle tetrimino events.
     * @details
     * On `RIGHT`/`LEFT` key press moves the tetrimino to the right or to the left
     * respectively and sets horizontal speed.
     * On `ACC` key hold increases the falling speed.
     * On `DROP` key press drops the tetrimino and moves the blocks to the
     * TetrisField.
     * On `ROT_CCW` or `ROT_CW` key press rotates the tetrimino counter-clockwise or
     * clockwise respectively and sets rotation speed.
     */
    void handle_event(Game &game, const SDL_Event &e);

    /**
     * @brief If initialized, move the tetrimino downwards if enough time has passed.
     * @details
     * If there was a field block beneath before the call, moves the blocks to the
     * tetris field.
     * @note Moves no more than one block at a time.
     * @param dt Movement time.
     * @return `true` if the tetrimino reached a field block beneath and was released
     *     or if the tetrimino has not been spawned before.
     */
    bool fall(int dt);

    /**
     * @brief If initialized, rotate and move the tetrimino horizontaly.
     * @note Can rotate more than once and move more than one block.
     * @param dt Movement time.
     */
    void move(int dt);

    /// Get the current tetrimino config.
    TetriminoConfig get_config() const;

private:
    /// Blocks per second side movement speed.
    static constexpr int TETRIMINO_SIDE_SPEED = 7;

    /// Rotations per second rotation speed.
    static constexpr int TETRIMINO_ROT_SPEED = 4;
    
    /// All tetrimino schemes. `1` stands for block, `0` stands for no block.
    static std::vector<std::vector<Scheme>> schemes;

    static std::vector<SDL_Rect> blockClips; /// The texture sheet clips.

    /**
     * @brief Shift the tetrimino by `dx` if it does not cause a field collision.
     * @note Might move the tetrimino through blocks if `dx` is large enough.
     */
    void shift(int dx);

    /**
     * @brief Move the tetrimino down until a field block is reached.
     * @note Does not stop the tetrimino.
     */
    void drop();

    /// `true` if the tetrimino was rotated by `dir` after shifting by `dx`, `dy`.
    bool check_adjacent(int dir, int dx, int dy);

    /**
     * @brief Rotate the tetrimino if it doesn't create a collision.
     * @param dir Amount of rotations. Positive values rotate counter-clockwise,
     *     negative values rotate clockwise.
     * @param checkAdjacent If `true` and rotation causes a collision, tries to shift
     *     the tetrimino once or twice before rotating.
     * @return `true` if the tetrimino was rotated.
     */
    bool rotate(int dir, bool checkAdjacent=true);

    /// Check if the tetrimino overlaps with a field block or exceeds the left wall.
    bool check_collision_left();

    /// Check if the tetrimino overlaps with a field block or exceeds the right wall.
    bool check_collision_right();

    /// Check if there is a field block or the field bottom under the lowest block.
    bool check_collision_bottom();

    /// Move all blocks to the field.
    void stop();

    Texture *blockTextureSheet;
    TetrisField *field;
    KeyLayout *keyLayout;
    TetriminoType type; /// Current tetrimino type.
    TetriminoRotation rot; /// Current rotation.
    int totalBlocks; /// Amount of blocks the current scheme has.
    int posX, posY; /// Field position coordinates.
    int fallDelay, fallElapsed;
    int sideVel, sideElapsed;
    int rotVel, rotElapsed;
    std::vector<Block *> blocks;

    /// Schemes for all rotations for the current type.
    std::vector<Scheme> *rotations;
};

/// A struct for easy storage of a tetrimino type and rotation.
struct TetriminoConfig
{
    /// Create a random config.
    TetriminoConfig();

    /// Create a config with given parameters.
    TetriminoConfig(Tetrimino::TetriminoType type, Tetrimino::TetriminoRotation rot);
    
    Tetrimino::TetriminoType type;
    Tetrimino::TetriminoRotation rot;
};


#endif
