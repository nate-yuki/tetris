/**
 * @file  font.hpp
 * @brief Include file for Font class.
 */

#ifndef FONT_HPP
#define FONT_HPP


#include "util.hpp"

#include <SDL2/SDL_ttf.h>
#include <string>


/// SDL_Font wrapper class.
class Font
{
public:
    /**
     * @brief Load the font from `path` with point size `ptSize`.
     * @throws `ExceptionSDL` thrown if font could not be created.
     */
    void init(const std::string &path, int ptSize);

    /// Destroy the font.
    void free();

    /**
     * @brief Create an SDL surface from `text` with `color`.
     * @param text The text.
     * @param color Text color.
     * @return A pointer to the created surface.
     * @throws `ExceptionSDL` thrown if surface could not be created.
     */
    SDL_Surface *create_text_surface(const std::string &text, const Color &color);

    /// Set font point size.
    void set_size(int ptSize);

    /// Get font point size.
    int get_size() const;

    /// Get the total font height.
    int get_height() const;

    /// Calculate how much of `text` characters will fit in `width` without rendering.
    int fit_in_width(const std::string &text, int width);

    /// Get the width of `text` when rendered without rendering.
    int get_text_width(const std::string &text);

    /// Get the height of `text` when rendered without rendering.
    int get_text_height(const std::string &text);

    /**
     * @brief Determine whether `text` will fit in a rectangle of size `w`x`h`.
     * @note Does not render text.
     * @note Takes newlines into account.
     */
    bool text_fits(const std::string &text, int w, int h);

    /**
     * @brief Increase font point size until `text` just fits in a `w`x`h` rectangle.
     * @details
     * Goes through point sizes from `minPtSize` to `MAX_PT_SIZE`. Resulting size is
     * the last tried one that fits `text` into the given dimensions with newlines
     * taken into account. If `text` does not fit with `minPtSize`, goes back to the
     * point size before the call. If `minPtSize` > `MAX_PT_SIZE`, does nothing.
     * @note Does not render text.
     * @param text The text.
     * @param w Maximum width.
     * @param h Maximum height.
     * @param minPtSize Point size to start increasing from.
     * @return New point size.
     */
    int fit_font(const std::string &text, int w, int h, int minPtSize=1);

private:
    TTF_Font *font = NULL;

    /// Keeps track of point size as there is no way to obtain it with SDL.
    int ptSize;
};


#endif
