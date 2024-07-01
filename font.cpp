/**
 * @file  font.cpp
 * @brief Implementation of the class Font.
 */

#include "font.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <algorithm>


void Font::init (const std::string &path, int ptSize)
{
    log("Initializing font", __FILE__, __LINE__);

    font = TTF_OpenFont(path.c_str(), ptSize);
    if (font == NULL)
    {
        throw ExceptionSDL(__FILE__, __LINE__, TTF_GetError());
    }
    this->ptSize = ptSize;
}

void Font::free ()
{
    log("Freeing font", __FILE__, __LINE__);

    TTF_CloseFont(font);
    font = NULL;
}

SDL_Surface *Font::create_text_surface (const std::string &text, const Color &color)
{
    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(
        font, text.c_str(), {color.r, color.g, color.b, 255}, 0
    );
    if (surface == NULL)
    {
        throw ExceptionSDL(__FILE__, __LINE__, TTF_GetError());
    }

    return surface;
}

void Font::set_size (int ptSize)
{
    TTF_SetFontSize(font, ptSize);
    this->ptSize = ptSize;
}

int Font::get_size () const
{
    return ptSize;
}

int Font::get_height () const
{
    return TTF_FontHeight(font);
}

int Font::fit_in_width (const std::string &text, int width)
{
    int extent, count;
    TTF_MeasureUTF8(font, text.c_str(), width, &extent, &count);
    return extent;
}

int Font::get_text_width (const std::string &text)
{
    int w, h;
    TTF_SizeUTF8(font, text.c_str(), &w, &h);
    return w;
}

int Font::get_text_height (const std::string &text)
{
    int w, h;
    TTF_SizeUTF8(font, text.c_str(), &w, &h);
    return h;
}

bool Font::text_fits (const std::string &text, int w, int h)
{
    int actualW, actualH;
    TTF_SizeUTF8(font, text.c_str(), &actualW, &actualH);
    actualH *= 1 + std::count(text.begin(), text.end(), '\n');
    
    return actualW <= w && actualH <= h;
}

int Font::fit_font (const std::string &text, int w, int h, int minPtSize)
{
    if (minPtSize > MAX_PT_SIZE)
    {
        return ptSize;
    }

    // Return to initial point size if text does not fit with minPtSize
    int currPtSize = ptSize;
    set_size(minPtSize);
    if (!text_fits(text, w, h))
    {
        log(
            "Point size " + std::to_string(minPtSize) + " is to small to fit \""
            + text + "\" into " + std::to_string(w) + "x" + std::to_string(h)
            + ". Going back to point size " + std::to_string(currPtSize),
            __FILE__, __LINE__
        );
        
        set_size(currPtSize);
        return ptSize;
    }

    log(
        "Fitting \"" + text + "\" from point size " + std::to_string(minPtSize)
            + "...",
        __FILE__, __LINE__
    );

    currPtSize = minPtSize;
    do {
        set_size(++currPtSize);
    }
    while (text_fits(text, w, h) && currPtSize <= MAX_PT_SIZE);
    set_size(--currPtSize);

    log(
        "...fitted \"" + text + "\" to point size " + std::to_string(currPtSize),
        __FILE__, __LINE__
    );

    return currPtSize;
}
