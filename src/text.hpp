/**
 * @file  text.hpp
 * @brief Include file for Text class.
 */

#ifndef TEXT_HPP
#define TEXT_HPP


#include "renderer.hpp"
#include "texture.hpp"
#include "font.hpp"


/// Text wrapper class with simple and efficient rendering.
class Text
{
public:
    enum TextAlign {
        TEXT_CENTER, // Place text in the center.
        TEXT_CENTER_TOP, // Center text and place it adjacent to the top border.
        TEXT_CENTER_BOTTOM, // Center text and place it adjacent to the bottom border.
        TEXT_CENTER_LEFT, // Center text and place it adjacent to the left border.
        TEXT_CENTER_RIGHT, // Center text and place it adjacent to the right border.
        TEXT_UPPER_LEFT, // Place text in the upper left corner.
        TEXT_UPPER_RIGHT, // Place text in the upper right corner.
        TEXT_BOTTOM_LEFT, // Place text in the bottom left corner.
        TEXT_BOTTOM_RIGHT, // Place text in the bottom right corner.
    };

    /**
     * @brief Initialize class members. Does not do any rendering.
     * @param renderer Renderer to use for text rendering.
     * @param font Font to use for text.
     * @param text The displayed text.
     * @param color Text color.
     * @param maxText Text to use for font fitting. Useful when consistent size for
     *     different text is needed. By default, `text` is used.
     */
    void init(
        Renderer &renderer, Font &font, const std::string &text, const Color &color,
        const std::string &maxText=""
    );
    
    /// Free the texture.
    void free();

    /**
     * @brief Render text so that it just fits in a `w`x`h` rectangle.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w Maximum width.
     * @param h Maximum height.
     * @param centering Position to place the text in relative to the rectangle.
     * @note If `w` and `h` are the same since the previous call, uses the same
     *     texture.
     * @note If called for the first time or the dimensions have changed,
     *     fits the font point size starting from 1 if the current size is too small,
     *     or from current size + 1 if `text` already fits.
     *     If this results in a point size change, creates a new texture.
     */
    void render(int x, int y, int w, int h, TextAlign centering=TEXT_CENTER);

    /**
     * @brief Set new text.
     * @param text The new text.
     * @param color New color; by default, does not change collor.
     * @note
     * Creates a new texture after setting `font` point size to the last one used for
     * rendering by this object. Should not be called before a first `render` call on
     * this object.
     * @note `text` should fit in the same dimensions as `maxText`.
     */
    void set_text(const std::string &text, const Color *color=nullptr);
    
    /// Get texture width.
    int get_width() const;
    
    /// Get texture height.
    int get_height() const;

private:
    Renderer *renderer;
    Font *font;
    std::string text, maxText;
    Color color;
    Texture texture;
    int lastW, lastH, lastPtSize;
};


#endif
