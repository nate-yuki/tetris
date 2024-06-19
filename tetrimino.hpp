#ifndef TETRIMINO_HPP
#define TETRIMINO_HPP


#include "tetris_field.hpp"
#include "texture.hpp"
#include "util.hpp"

#include <vector>
#include <string>


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

    static std::vector<std::vector<Scheme>> schemes;
    static std::vector<SDL_Rect> blockClips;

    static void load_schemes(const std::string &path);
    static void init_clips();

    void init(
        int posX, int posY, int moveDelay, Texture &blockTextureSheet,
        TetriminoType type, TetriminoRotation rot=TETRIMINO_ROTATION_0
    );

    void render(int x, int y, int size);

    bool move(TetrisField &field, int dt);

private:
    bool check_collision(TetrisField &field);
    void stop(TetrisField &field);

    TetriminoType type;
    TetriminoRotation rot;
    int totalBlocks;
    int posX, posY;
    int moveDelay, elapsed;
    std::vector<Block *> blocks;
    std::vector<Scheme> *rotations;
};


#endif
