#ifndef PARTICLES_HPP
#define PARTICLES_HPP


#include "texture.hpp"


class Particle
{
public:
    static void init_clips();

    Particle(int lifespan, int maxShift, Texture *particleTextureSheet);
    void render(int x, int y, int size);
    bool is_dead() const;

private:
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

    static std::vector<SDL_Rect> clips;

    int xShift, yShift;
    int frame, lifespan;
    Texture *particleTextureSheet;
    SDL_Rect *clip;
};

class ParticleEmmiter
{
public:
    ParticleEmmiter(
        int particlesMax, int lifespan, int maxShift, Texture *particleTextureSheet
    );
    ~ParticleEmmiter();

    void render(int x, int y, int w, int h, int size);

private:
    int particlesTotal;
    std::vector<Particle *> particles;
    int lifespan;
    int maxShift;
    Texture *particleTextureSheet;
};


#endif
