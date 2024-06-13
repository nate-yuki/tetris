/**
 * @file  texture.hpp
 * @brief Include file for Texture class.
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include "renderer.hpp"
#include "font.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>


/// SDL_Texture wrapper class.
class Texture
{
public:
    ~Texture();

    /// Destroy texture if it was not destroyed.
    void free();

    /**
     * @brief Load and color key a texture from the image stored in `path` using
     *     `renderer`.
     * @note Stores a pointer to `renderer`.
     * @param renderer Renderer to use for texture creation.
     * @param path Image path.
     * @param keyColor The transparent pixel; `nullptr` to not use color keying.
     */
    void load_from_file(
        Renderer &renderer, const std::string &path, const Color *keyColor=nullptr
    );

    /**
     * @brief Create texture from `text` using `renderer` and `font`.
     * @note Stores pointers to `renderer` and `font`.
     * @param renderer Renderer to use for texture creation.
     * @param font Font to use to create a text surface.
     * @param text The text.
     * @param color Text color.
     */
    void load_from_text(
        Renderer &renderer, Font &font, const std::string &text,
        const Color &color={0, 0, 0}
    );

    /// Set texture color mode.
    void set_color(const Color *color);

    /// Set texture blend mode.
    void set_blend_mode(SDL_BlendMode blending);

    /// Set texture transparency.
    void set_alpha(Uint8 alpha);

    /**
     * @brief Render a `clip` from the texture with upper left corner at (`x`, `y`).
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param clip Portion of the texture to render; `NULL` to render the entire
     *     texture.
     * @param angle Angle in degrees to rotate the texture.
     * @param center Point to rotate around;
     *     `NULL` to rotate around the texture center.
     * @param flip SDL_RendererFlip value setting flipping actions.
     */
    void render(
        int x, int y, const SDL_Rect *clip=NULL,
        double angle=0.0, const SDL_Point *center=NULL,
        SDL_RendererFlip flip=SDL_FLIP_NONE
    );

    /**
     * @brief Render a `clip` from the texture to `renderQuad` with possible
     *     stretching.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param renderQuad Portion of the current scene to render to;
     *     NULL to render to the entire scene.
     * @param angle Angle in degrees to rotate the texture.
     * @param center Point to rotate around;
     *     `NULL` to rotate around the texture center.
     * @param flip SDL_RendererFlip value setting flipping actions.
     */
    void render(
        const SDL_Rect &renderQuad, const SDL_Rect *clip=NULL,
        double angle=0.0, const SDL_Point *center=NULL,
        SDL_RendererFlip flip=SDL_FLIP_NONE
    );

    /// Get texture width.
    int get_width() const;

    /// Get texture height.
    int get_height() const;

private:
    SDL_Texture *texture = NULL;
    Renderer *renderer = nullptr;
    int w, h;
};


#endif
