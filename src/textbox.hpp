/**
 * @file  textbox.hpp
 * @brief Include file for Textbox class.
 */

#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP


#include "texture.hpp"
#include "shapes.hpp"
#include "text.hpp"
#include "util.hpp"


/// A resizable rectangle containing text.
class TextBox
{
public:
    /**
     * @brief Initialize the underlying `Text` and `Box` objects.
     * @param renderer Renderer to use for rendering.
     * @param font Font to use for text.
     * @param line The displayed text.
     * @param fillColor Fill color.
     * @param frameColor Frame color.
     * @param textColor Text color.
     * @param maxText Text to use for font fitting; by default, `text` is used.
     */
    void init(
        Renderer &renderer, Font &font, const std::string &line,
        const Color &fillColor, const Color &frameColor,
        const Color &textColor=BLACK, const std::string &maxText=""
    );

    /// Free the text.
    void free();

    /**
     * @brief Draw the text box with given location and dimensions.
     * @details
     * Previously set text and colors are used.
     * Text is sized so that it just fits inside the frame.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param w Bounding box width.
     * @param h Bounding box height.
     * @param thickness Frame width.
     */
    void render(int x, int y, int w, int h, int thickness);
    
    /// Set the frame color.
    void set_frame_color(const Color &color);
    
    /// Set fill color.
    void set_fill_color(const Color &color);

    /**
     * @brief Set new text. `line` should fit in the same dimensions as `maxText`.
     * @param line The new text.
     * @param color New color; by default, does not change collor.
     */
    void set_text(const std::string &line, const Color *color=nullptr);

private:
    Text text;
    Box box;
};


#endif
