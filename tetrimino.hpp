#ifndef TETRIMINO_HPP
#define TETRIMINO_HPP


#include "tetris_field.hpp"
#include "texture.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <string>


struct TetriminoConfig;

class Block;
class TetrisField;

class Tetrimino
{
public:
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

    enum TetriminoRotation {
        TETRIMINO_ROTATION_0,
        TETRIMINO_ROTATION_90,
        TETRIMINO_ROTATION_180,
        TETRIMINO_ROTATION_270,
        TETRIMINO_ROTATION_TOTAL,
    };

    static void load_schemes(const std::string &path);
    static void init_clips();
    static void render_config(
        const TetriminoConfig &config, int x, int y, int size,
        Texture *blockTextureSheet
    );

    void init(TetrisField *field, Texture *blockTextureSheet);

    void free();

    bool spawn(
        int posX, int posY, int fallDelay, const TetriminoConfig &config,
        bool ghost=false
    );

    void render(int x, int y, int size);
    void handle_event(Game &game, const SDL_Event &e);

    bool fall(int dt);
    void move(int dt);

private:
    static constexpr int TETRIMINO_SIDE_SPEED = 7;
    static constexpr int TETRIMINO_ROT_SPEED = 4;
    
    static std::vector<std::vector<Scheme>> schemes;
    static std::vector<SDL_Rect> blockClips;

    void shift(int dx);
    void drop();
    void rotate(int dir);
    bool check_collision_left();
    bool check_collision_right();
    bool check_collision_bottom();
    void stop();

    Texture *blockTextureSheet;
    TetrisField *field;
    TetriminoType type;
    TetriminoRotation rot;
    int totalBlocks;
    int posX, posY;
    int fallDelay, fallElapsed;
    int sideVel, sideElapsed;
    int rotVel, rotElapsed;
    std::vector<Block *> blocks;
    std::vector<Scheme> *rotations;
};

struct TetriminoConfig
{
    TetriminoConfig();
    TetriminoConfig(Tetrimino::TetriminoType type, Tetrimino::TetriminoRotation rot);
    
    Tetrimino::TetriminoType type;
    Tetrimino::TetriminoRotation rot;
};


#endif
