/**
 * @file  text.cpp
 * @brief Implementation of the class Text.
 */

#include "text.hpp"
#include "logger.hpp"


void Text::init (
    Renderer &renderer, Font &font, const std::string &text, const Color &color,
    const std::string &maxText
)
{
    log("Initializing Text from \"" + text + "\"", __FILE__, __LINE__);

    this->renderer = &renderer;
    this->font = &font;
    this->text = text;
    this->color = color;
    this->maxText = maxText.size() ? maxText : text;
    lastW = lastH = -1;
    lastPtSize = 1;
}

void Text::free ()
{
    texture.free();
}

void Text::render (int x, int y, int w, int h, TextAlign centering)
{
    if (w != lastW || h != lastH)
    {
        if (font->text_fits(maxText, w, h))
        {
            font->fit_font(maxText, w, h, font->get_size() + 1);
        }
        else
        {
            font->fit_font(maxText, w, h);
        }
        if (font->get_size() != lastPtSize)
        {
            texture.load_from_text(*renderer, *font, text, color);
        }
        lastW = w;
        lastH = h;
        lastPtSize = font->get_size();
    }
    switch (centering)
    {
    case TEXT_CENTER:
        texture.render(
            x + (w - texture.get_width()) / 2, y + (h - texture.get_height()) / 2
        );
        break;

    case TEXT_CENTER_TOP:
        texture.render(x + (w - texture.get_width()) / 2, y);
        break;

    case TEXT_CENTER_BOTTOM:
        texture.render(
            x + (w - texture.get_width()) / 2, y + h - texture.get_height()
        );
        break;

    case TEXT_CENTER_LEFT:
        texture.render(x, y + (h - texture.get_height()) / 2);
        break;

    case TEXT_CENTER_RIGHT:
        texture.render(
            x + w - texture.get_width(), y + (h - texture.get_height()) / 2
        );
        break;

    case TEXT_UPPER_LEFT:
        texture.render(x, y);
        break;

    case TEXT_UPPER_RIGHT:
        texture.render(x + w - texture.get_width(), y);
        break;

    case TEXT_BOTTOM_LEFT:
        texture.render(x, y + h - texture.get_height());
        break;

    case TEXT_BOTTOM_RIGHT:
        texture.render(x + w - texture.get_width(), y + h - texture.get_height());
        break;
    }
}

void Text::set_text (const std::string &text)
{
    this->text = text;
    font->set_size(lastPtSize);
    texture.load_from_text(*renderer, *font, text, color);
}

int Text::get_width () const
{
    return texture.get_width();
}

int Text::get_height () const
{
    return texture.get_height();
}
