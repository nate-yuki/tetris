#include "particles.hpp"
#include "util.hpp"
#include "constants.hpp"
#include "logger.hpp"

#include <cmath>


std::vector<SDL_Rect> Particle::clips;


void Particle::init_clips ()
{
    for (int type = 0; type < PARTICLE_TOTAL; ++type)
    {
        clips.push_back(
            {type * MAX_PARTICLE_SIZE, 0, MAX_PARTICLE_SIZE, MAX_PARTICLE_SIZE}
        );
    }
}

Particle::Particle (int maxShift, int lifespan, Texture *particleTextureSheet)
    : xShift(rand() % (2 * maxShift) - maxShift)
    , yShift(rand() % (2 * maxShift) - maxShift)
    , frame(rand() % lifespan)
    , lifespan(lifespan)
    , particleTextureSheet(particleTextureSheet)
    , clip(&clips[1 + rand() % (PARTICLE_TOTAL - 1)])
{}

void Particle::render (int x, int y, int size)
{
    particleTextureSheet->render({x + xShift, y + yShift, size, size}, clip);
    if (frame % 2)
    {
        particleTextureSheet->render(
            {x + xShift, y + yShift, size, size}, &clips[PARTICLE_SHIMMER]
        );
    }
    ++frame;
}

bool Particle::is_dead () const
{
    return frame >= lifespan;
}


ParticleEmmiter::ParticleEmmiter (
    int particlesMax, int lifespan, int maxShift, Texture *particleTextureSheet
)
    : particlesTotal(particlesMax)
    , particles(std::vector<Particle *>(particlesTotal))
    , lifespan(lifespan)
    , maxShift(maxShift)
    , particleTextureSheet(particleTextureSheet)
{
    for (Particle *&particle : particles)
    {
        particle = new Particle(maxShift, lifespan, particleTextureSheet);
    }
}

ParticleEmmiter::~ParticleEmmiter ()
{
    for (Particle *&particle : particles)
    {
        delete particle;
    }
}

void ParticleEmmiter::render (int x, int y, int w, int h, int size)
{
    double numenator = (
        std::sqrt(w * w + h * h + w * h * (4 * particlesTotal - 2)) - w - h
    ) / 2;
    int particlesHor = max(1, 1 + numenator / h);
    int particlesVer = max(1, 1 + numenator / w);

    int spaceHor, spaceVer;
    if (particlesHor == 1)
    {
        y += h / 2;
        spaceHor = 0;
    }
    else
    {
        spaceHor = w / (particlesHor - 1);
    }
    if (particlesVer == 1)
    {
        x += w / 2;
        spaceVer = 0;
    }
    else
    {
        spaceVer = h / (particlesVer - 1);
    }

    /*log(
        "particlesTotal " + std::to_string(particlesTotal) +
        " particlesHor = " + std::to_string(particlesHor) +
        " particlesVer = " + std::to_string(particlesVer) +
        " spaceHor = " + std::to_string(spaceHor) +
        " spaceVer = " + std::to_string(spaceVer) +
        " numenator = " + std::to_string(numenator),
        __FILE__, __LINE__
    );
    */
    for (int row = 0; row < particlesVer; ++row)
    {
        for (int col = 0; col < particlesHor; ++col)
        {
            if (particles[row * particlesHor + col]->is_dead())
            {
                delete particles[row * particlesHor + col];
                particles[row * particlesHor + col] = new Particle(
                    maxShift, lifespan, particleTextureSheet
                );
            }
            particles[row * particlesHor + col]->render(
                x + col * spaceHor, y + row * spaceVer, size
            );
        }
    }
}
