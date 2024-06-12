/**
 * @file  renderer.cpp
 * @brief Implementation of the class Renderer.
 */

#include "renderer.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <SDL2/SDL_image.h>


void Renderer::init (Window &window)
{
    log("Initializing Renderer", __FILE__, __LINE__);

    renderer = window.create_renderer();
}

void Renderer::free ()
{
    log("Freeing renderer", __FILE__, __LINE__);

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
}

void Renderer::show ()
{
    SDL_RenderPresent(renderer);
}

void Renderer::clear ()
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}

SDL_Texture *Renderer::create_texture_from_file (const std::string &path)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == NULL)
    {
        throw ExceptionSDL(__FILE__, __LINE__, SDL_GetError());
    }
    
    return texture;
}

SDL_Texture *Renderer::create_texture_from_surface (SDL_Surface *surface)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
        throw ExceptionSDL(__FILE__, __LINE__, SDL_GetError());
    }
    
    return texture;
}

void Renderer::render_texture (
    SDL_Texture *texture, const SDL_Rect *clip, const SDL_Rect *renderQuad,
    double angle, const SDL_Point *center, SDL_RendererFlip flip
)
{
    SDL_RenderCopyEx(renderer, texture, clip, renderQuad, angle, center, flip);
}

void Renderer::render_rectangle(const SDL_Rect *rectangle, const Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, rectangle);
}

void Renderer::set_view_port (int x, int y, int w, int h)
{
    SDL_Rect viewPort = {x, y, w, h};
    SDL_RenderSetViewport(renderer, &viewPort);
}

void Renderer::reset_view_port ()
{
    SDL_RenderSetViewport(renderer, NULL);
}

int Renderer::get_width () const
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return w;
}

int Renderer::get_height () const
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    return h;
}
