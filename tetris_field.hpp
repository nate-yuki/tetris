/**
 * @file  tetris_field.hpp
 * @brief Include file for TetrisField and Block classes.
 */

#ifndef TETRIS_FIELD_HPP
#define TETRIS_FIELD_HPP


#include "texture.hpp"
#include "tetrimino.hpp"
#include "particles.hpp"

#include <SDL2/SDL.h>
#include <list>
#include <vector>


class Tetrimino;

/// A tetrimino block class.
class Block
{
public:
    /**
     * @brief Initialize class members.
     * @param blockTextureSheet A block textures sheet.
     * @param clip A clip from `blockTextureSheet` to use as the block's texture.
     */
    Block(Texture *blockTextureSheet, const SDL_Rect *clip);

    /**
     * @brief Draw the block with given location and dimensions.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param size Block size.
     */
    void render(int x, int y, int size);

private:
    Texture *blockTextureSheet;
    const SDL_Rect *clip;
};

/// A tetrimino block grid
class TetrisField
{
public:
    /**
     * @brief Create an empty field.
     * @param cellsHor Amount of cells in each row.
     * @param cellsVer Amount of cells in each column.
     * @param bgTexture Empty cell texture.
     * @param frameTexture Texture to render behind the field (see `render()`).
     * @param clearTexture Texture to use for cleared rows.
     * @param particleTextureSheet Particle texture sheet to use for cleared row
     *     particles.
     */
    void init(
        int cellsHor, int cellsVer,
        Texture *bgTexture, Texture *frameTexture, Texture *clearTexture,
        Texture *particleTextureSheet
    );

    /// Free all blocks.
    void free();

    /**
     * @brief Render the field and a `tetrimino` with given parameters.
     * @details
     * `frameTexture` is rendered to the entire rectangle defined by `x`, `y`, `w`,
     * `h`. The grid is centered and rendered above it with size calculated to fit
     * into both `w` and `h`. If `w`/`h` = `cellsHor`/`cellsVer`, the grid will fill
     * out the entire rectangle.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w The width.
     * @param h The height.
     * @param tetrimino Tetrimino to render.
     * @param stopClearLineRender If `true` and a cleared line is being rendered,
     *     stops rendering the cleared line.
     */
    void render(
        int x, int y, int w, int h, Tetrimino &tetrimino, bool stopClearLineRender
    );

    /// `true` if the field has a block in column `posX`, row `posY`.
    bool has_block(int posX, int posY) const;

    /// Get the amount of cells in each row.
    int get_width() const;

    /// Get the amount of cells in each column.
    int get_height() const;

    /// Store a `block` in column `posX`, row `posY`.
    void add_block(int posX, int posY, Block *block);

    /**
     * @brief Find all rows filled with blocks and remove them.
     * @return The amount of cleared lines.
     */
    int clear_lines();

private:
    Texture *bgTexture, *frameTexture, *clearTexture, *particleTextureSheet;
    std::vector<ParticleEmmiter *> clearLineParticlers;
    std::vector<std::vector<Block *>> field;
    int cellsHor, cellsVer;

    /// Cleared line indeces. `-1` is always stored as the last element.
    std::vector<int> clearedLines;
};


#endif
