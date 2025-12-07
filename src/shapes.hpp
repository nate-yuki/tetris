/**
 * @file  shapes.hpp
 * @brief Include file for different shape classes.
 */

#ifndef SHAPES_HPP
#define SHAPES_HPP


#include "renderer.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>


/// Basic SDL rectangle wrapper class.
class Rectangle
{
public:
    /**
     * @brief Initialize class members.
     * @param renderer Renderer to use for rendering.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w The width.
     * @param h The height.
     * @param color Fill color.
     */
    void init(Renderer &renderer, int x, int y, int w, int h, const Color &color);
    
    /**
     * @brief Draw the rectangle.
     * @details
     * Uses the current color and the coordinates and size it was initialized with.
     */
    void render();

    /// Set fill color.
    void set_color(const Color &color);

private:
    Renderer *renderer;
    SDL_Rect rectangle;
    Color color;
};

/// A hollow frame class.
class Frame
{
public:
    /**
     * @brief Initialize rectangles the frame consists of.
     * @param renderer Renderer to use for rendering.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w Bounding box width.
     * @param h Bounding box height.
     * @param thickness Frame width.
     * @param color Frame color.
     */
    void init(
        Renderer &renderer,
        int x, int y, int w, int h, int thickness, const Color &color
    );
    
    /**
     * @brief Draw the frame.
     * @details
     * Uses the current color and the other parameters it was initialized with.
     */
    void render();

    /// Set frame color.
    void set_color(const Color &color);

private:
    Rectangle up, down, left, right;
};

/// A bordered rectangle that can be rendered with different parameters.
class Box
{
public:
    /**
     * @brief Initialize class members.
     * @param renderer Renderer to use for rendering.
     * @param fillColor Fill color.
     * @param frameColor Frame color.
     */
    void init(Renderer &renderer, const Color &fillColor, const Color &frameColor);
    
    /// Set frame color.
    void set_frame_color(const Color &color);
    
    /// Set fill color.
    void set_fill_color(const Color &color);

    /**
     * @brief Draw the box with given location and dimensions using previously set
     *     colors.
     * @note
     * If the location and dimensions are the same since the previous call, uses the
     * same underlying rectangles. If called for the first time or the parameters have
     * changed, creates new rectangles. If only the thickness has changed, only
     * creates a new frame.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w Bounding box width.
     * @param h Bounding box height.
     * @param thickness Frame width.
     */
    void render(int x, int y, int w, int h, int thickness);

private:
    Renderer *renderer;
    Rectangle fill;
    Frame frame;
    int x, y, w, h, thickness;
    Color fillColor, frameColor;
};


#endif
