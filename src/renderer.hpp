/**
 * @file  renderer.hpp
 * @brief Include file for Renderer class.
 */

#ifndef RENDERER_HPP
#define RENDERER_HPP


#include "window.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <string>


/// SDL_Renderer wrapper class.
class Renderer
{
public:
    /// Create a renderer attached to `window`.
    void init(Window &window);

    /// Destroy renderer if it was not destroyed.
    void free();

    /// Display the current scene.
    void show();

    /// Clear the scene creating a white background.
    void clear();

    /**
     * @brief Create an SDL texture from the image in `path`.
     * @param path Image path.
     * @return A pointer to the created texture.
     * @throws `ExceptionSDL` thrown if the texture could not be created.
     */
    SDL_Texture *create_texture_from_file(const std::string &path);

    /**
     * @brief Create an SDL texture from an SDL `surface`.
     * @param surface The surface.
     * @return A pointer to the created texture.
     * @throws `ExceptionSDL` thrown if the texture could not be created.
     */
    SDL_Texture *create_texture_from_surface(SDL_Surface *surface);

    /**
     * @brief Render a `texture` `clip` at `renderQuad`.
     * @param texture The texture.
     * @param clip Portion of the texture to render; `NULL` to render the entire
     *     texture.
     * @param renderQuad Portion of the current scene to render to;
     *     `NULL` to render to the entire scene.
     * @param angle Angle in degrees to rotate the texture.
     * @param center Point to rotate around;
     *     `NULL` to rotate around the texture center.
     * @param flip SDL_RendererFlip value setting flipping actions.
     */
    void render_texture(
        SDL_Texture *texture, const SDL_Rect *clip=NULL,
        const SDL_Rect *renderQuad=NULL,
        double angle=0.0, const SDL_Point *center=NULL,
        SDL_RendererFlip flip=SDL_FLIP_NONE
    );

    /// Render rectangle at `rectangle` filled with `color`.
    void render_rectangle(const SDL_Rect *rectangle, const Color &color);

    /// Set view port.
    void set_view_port(int x, int y, int w, int h);

    /// Reset view port to the whole window.
    void reset_view_port();

    /// Get output width.
    int get_width() const;

    /// Get output height.
    int get_height() const;

private:
    SDL_Renderer *renderer = NULL;
};


#endif
