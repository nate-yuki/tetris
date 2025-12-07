/**
 * @file  shapes.cpp
 * @brief Implementation of different shape classes.
 */

#include "shapes.hpp"
#include "logger.hpp"


void Rectangle::init (
    Renderer &renderer, int x, int y, int w, int h, const Color &color
)
{
    rectangle = {x, y, w, h};
    this->color = color;
    this->renderer = &renderer;
}

void Rectangle::render ()
{
    renderer->render_rectangle(&rectangle, color);
}

void Rectangle::set_color (const Color &color)
{
    this->color = color;
}


void Frame::init (
    Renderer &renderer, int x, int y, int w, int h, int thickness, const Color &color
)
{
    log("Initializing Frame", __FILE__, __LINE__);

    up.init(renderer, x, y, w, thickness, color);
    down.init(renderer, x, y + h - thickness, w, thickness, color);
    left.init(renderer, x, y + thickness, thickness, h - 2 * thickness, color);
    right.init(
        renderer,
        x + w - thickness, y + thickness, thickness, h - 2 * thickness, color
    );
}

void Frame::render ()
{
    up.render();
    down.render();
    left.render();
    right.render();
}

void Frame::set_color (const Color &color)
{
    up.set_color(color);
    down.set_color(color);
    left.set_color(color);
    right.set_color(color);
}


void Box::init (Renderer &renderer, const Color &fillColor, const Color &frameColor)
{
    log("Initializing Box", __FILE__, __LINE__);

    this->renderer = &renderer;
    this->fillColor = fillColor;
    this->frameColor = frameColor;
    x = y = w = h = thickness = -1;
}

void Box::set_frame_color (const Color &color)
{
    frame.set_color(color);
    frameColor = color;
}

void Box::set_fill_color (const Color &color)
{
    fill.set_color(color);
    fillColor = color;
}

void Box::render(int x, int y, int w, int h, int thickness)
{
    if (x != this->x || y != this->y || w != this->w || h != this->h)
    {
        fill.init(*renderer, x, y, w, h, fillColor);
        frame.init(*renderer, x, y, w, h, thickness, frameColor);
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->thickness = thickness;
    }
    else if (thickness != this->thickness)
    {
        frame.init(*renderer, x, y, w, h, thickness, frameColor);
        this->thickness = thickness;
    }
    fill.render();
    frame.render();
}
