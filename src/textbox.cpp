/**
 * @file  textbox.cpp
 * @brief Implementation of the class Textbox.
 */

#include "textbox.hpp"
#include "logger.hpp"


void TextBox::init (
    Renderer &renderer, Font &font, const std::string &line,
    const Color &fillColor, const Color &frameColor, const Color &textColor,
    const std::string &maxText
)
{
    log("Initializing TextBox", __FILE__, __LINE__);

    text.init(renderer, font, line, textColor, maxText);
    box.init(renderer, fillColor, frameColor);
}

void TextBox::free ()
{
    text.free();
}

void TextBox::render (int x, int y, int w, int h, int thickness)
{
    box.render(x, y, w, h, thickness);
    // Text is fit not to overlap with the frame
    text.render(x + thickness, y + thickness, w - 2 * thickness, h - 2 * thickness);
}

void TextBox::set_frame_color (const Color &color)
{
    box.set_frame_color(color);
}

void TextBox::set_fill_color (const Color &color)
{
    box.set_fill_color(color);
}

void TextBox::set_text (const std::string &line, const Color *color)
{
    text.set_text(line, color);
}
