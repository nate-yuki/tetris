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
        TetriminoType type, Texture &blockTextureSheet, int posX, int posY,
        TetriminoRotation rot=TETRIMINO_ROTATION_0
    );

    void render(int x, int y, int s);

private:
    TetriminoType type;
    TetriminoRotation rot;
    int totalBlocks;
    int posX, posY;
    std::vector<Block *> blocks;
    std::vector<Scheme> *rotations;
};


#endif
