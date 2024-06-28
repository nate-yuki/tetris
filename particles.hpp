/**
 * @file  particles.hpp
 * @brief Include file for particle classes.
 */

#ifndef PARTICLES_HPP
#define PARTICLES_HPP


#include "texture.hpp"


/**
 * @brief A particle class.
 * @example
 * 
 *     Particle *particle = new Particle(60, 5, &particleTextureSheet);
 *     while (!particle.is_dead())
 *     {
 *         particle->render(10, 10, 5);
 *     }
 *     delete particle;
 */
class Particle
{
public:
    /// Create clips to use for selecting a particle texture from the sheet.
    static void init_clips();

    /**
     * @brief Create the particle.
     * @note
     * The particle will continue to render after its lifesycle elapses. The
     * livelihood should be checked manually via `is_dead()` call.
     * @param lifespan Maximum amount of frames to render the particle for. Actual
     *     lifespan can be lower, down to 0.
     * @param maxShift Maximum amount of pixels to shift the particle by in each
     *     direction from the rendering coordinate.
     * @param particleTextureSheet The texture sheet: first entry is the shimmer
     *     texture and all other are for different base textures.
     */
    Particle(int lifespan, int maxShift, Texture *particleTextureSheet);

    /**
     * @brief Draw the particle near the given location with given `size`.
     * @details
     * Advances the lifecycle. Every other frame the shimmer texture is rendered above
     * the base texture.
     * @param x Upper left corner x coordinate.
     * @param y Upper left corner y coordinate.
     * @param size The size.
     */
    void render(int x, int y, int size);

    /// `true` if the lifesycle is elapsed.
    bool is_dead() const;

private:
    /// Particle type values used for clip indexing.
    enum ParticleTypes {
        PARTICLE_SHIMMER,
        PARTICLE_RED,
        PARTICLE_GREEN,
        PARTICLE_BLUE,
        PARTICLE_WHITE,
        PARTICLE_BLACK,
        PARTICLE_YELLOW,
        PARTICLE_PURPLE,
        PARTICLE_TOTAL,
    };

    static std::vector<SDL_Rect> clips; /// The texture sheet clips.

    int xShift, yShift;
    int frame; /// Current animation frame.

    /// Amount of times the particle is rendered before being considered dead.
    int lifespan;
    
    Texture *particleTextureSheet;
    SDL_Rect *clip; /// This particle's texture sheet clip.
};

/// A particle emmiter class.
class ParticleEmmiter
{
public:
    /**
     * @brief Create the particles and initialize class members.
     * @param particlesMax Maximum amount of particles to render each time.
     * @param lifespan Maximum amount of frames to render each particle for. Actual
     *     lifespan is set randomly for each particle and can be 0.
     * @param maxShift Maximum amount of pixels to shift each particle by in each
     *     direction from its rendering coordinate.
     * @param particleTextureSheet The texture sheet.
     */
    ParticleEmmiter(
        int particlesMax, int lifespan, int maxShift, Texture *particleTextureSheet
    );

    /// Delete all particles.
    ~ParticleEmmiter();

    /**
     * @brief Uniformly render the particles with given `size` along a rectangle
     *     with given parameters.
     * @details
     * Amount of particles per row and column is chosen proportionally to `w`/`h`.
     * The particles are then rendered near uniformly distributed points starting from
     * the upper left corner down to the bottom right corner. If there is only one
     * particle in a row/column, the line of particles is centered along the
     * corresponding dimension. Dead particles are replaced with new ones.
     * @param x Rectangle upper left corner x coordinate.
     * @param y Rectangle upper left corner y coordinate.
     * @param w Rectangle width.
     * @param h Rectangle height.
     * @param size Particle size.
     */
    void render(int x, int y, int w, int h, int size);

private:
    int particlesTotal;
    std::vector<Particle *> particles;
    int lifespan;
    int maxShift;
    Texture *particleTextureSheet;
};


#endif
