/**
 * @file  texture.cpp
 * @brief Implementation of the class Texture.
 */

#include "texture.hpp"
#include "exceptions.hpp"
#include "logger.hpp"


Texture::~Texture ()
{
    free();
}

void Texture::free()
{
    if (texture != NULL)
    {
        log("Freeing Texture", __FILE__, __LINE__);

        SDL_DestroyTexture(texture);
        texture = NULL;
        renderer = nullptr;
        w = 0;
        h = 0;
    }
}

void Texture::load_from_file (
    Renderer &renderer, const std::string &path, const Color *keyColor
)
{
    log("Loading texture from \"" + path + "\"", __FILE__, __LINE__);

    // Destroy previous texture
    free();
    
    // If color keying is not needed, load texture straight from image
    if (keyColor == nullptr)
    {
        texture = renderer.create_texture_from_file(path);
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    }
    // Otherwise, load surface
    else
    {
        SDL_Surface *surface = IMG_Load(path.c_str());
        if (surface == NULL)
        {
            throw ExceptionSDL(__FILE__, __LINE__, IMG_GetError());
        }

        SDL_SetColorKey(
            surface, SDL_TRUE,
            SDL_MapRGB(surface->format, keyColor->r, keyColor->b, keyColor->g)
        );

        // Create texture from surface pixels
        texture = renderer.create_texture_from_surface(surface);
        w = surface->w;
        h = surface->h;
        SDL_FreeSurface(surface);
    }

    this->renderer = &renderer;
}

void Texture::load_from_text (
    Renderer &renderer, Font &font, const std::string &text, const Color &color
)
{
    log("Loading texture from text \"" + text + "\"", __FILE__, __LINE__);

    // Destroy previous texture
    free();

    SDL_Surface *surface = font.create_text_surface(text, color);
    
    // Create texture from surface pixels
    texture = renderer.create_texture_from_surface(surface);
    w = surface->w;
    h = surface->h;
    SDL_FreeSurface(surface);

    this->renderer = &renderer;
}

void Texture::set_color (const Color *color)
{
    SDL_SetTextureColorMod(texture, color->r, color->g, color->b);
}

void Texture::set_blend_mode (SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(texture, blending);
}

void Texture::set_alpha (Uint8 alpha)
{
    SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::render (
    int x, int y, const SDL_Rect *clip,
    double angle, const SDL_Point *center, SDL_RendererFlip flip
)
{
    // Render target size is set to texture size or to clip size if clip size is set
    SDL_Rect renderQuad = {x, y, w, h};
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    renderer->render_texture(texture, clip, &renderQuad, angle, center, flip);
}

int Texture::get_width () const
{
    return w;
}

int Texture::get_height () const
{
    return h;
}

