#define SDL_ENABLE_OLD_NAMES

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>

#include "SDL3/SDL.h"
#include "Klein.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "CollisionHandler.h"

// -------------------------------------------------------------------------
// Configurazione
// -------------------------------------------------------------------------

#define NUMBER_OF_ENTITIES  2000
#define ENTITY_RADIUS       25.f
#define SIMULATION_SECONDS  20.0


using namespace Klein;

// -------------------------------------------------------------------------
// Utility
// -------------------------------------------------------------------------

static Entity* makeEntity(float maxX, float maxY, unsigned int seed)
{
    srand(seed);

    Entity* e = new Entity();

    const point_t pos =
    {
        static_cast<float>(rand() % static_cast<int>(maxX)),
        static_cast<float>(rand() % static_cast<int>(maxY))
    };
    e->setPosition(pos);

    const vector_t vel =
    {
        (rand() % 200 - 100) / 100.f,
        (rand() % 200 - 100) / 100.f
    };
    e->setSpeed(vel);

    rectangle_t shape;
    shape.setHeight(rand() % MAX_PARTICLE_SIZE);
    shape.setWidth(rand() % MAX_PARTICLE_SIZE);
    shape.setCenter(pos);

    Hitbox* hb = new Hitbox(*e, shape);
    e->addHitbox(hb);

    return e;
}

// -------------------------------------------------------------------------
// main
// -------------------------------------------------------------------------

int main()
{
    const unsigned int baseSeed = static_cast<unsigned>(time(nullptr));
    std::cout << "Seed: "    << baseSeed          << '\n';
    std::cout << "Entita': " << NUMBER_OF_ENTITIES << '\n';
    std::cout << "Durata:  " << SIMULATION_SECONDS << " secondi\n\n";

    // -- Entità -------------------------------------------------------------
    std::vector<Entity*> entities;
    entities.reserve(NUMBER_OF_ENTITIES);
    for (int i = 0; i < NUMBER_OF_ENTITIES; i++)
        entities.push_back(makeEntity(SCREEN_WIDTH, SCREEN_HEIGHT, baseSeed + i));

    // -- Timer e handler ----------------------------------------------------
    TimeHandler timer;
    Entity::setTimer(&timer);

    CollisionHandler handler;
    handler.setEntitiesList(entities);

    // -- Game loop ----------------------------------------------------------
    int  totalCollisions = 0;
    int  frames          = 0;
    long long totalTime  = 0;

    const auto simStart = std::chrono::steady_clock::now();

    while (true)
    {
        // Controlla se il tempo è scaduto
        const auto now     = std::chrono::steady_clock::now();
        const double elapsed = std::chrono::duration<double>(now - simStart).count();
        if (elapsed >= SIMULATION_SECONDS) break;

        timer.tick();

        BEGIN_TIME;
        const int collisions = handler.runGridOptimization();
        END_TIME;

        totalCollisions += collisions;
        totalTime       += GET_TIME;
        frames++;
    }

    // -- Report finale ------------------------------------------------------
    std::cout << "Frames simulati:     " << frames          << '\n';
    std::cout << "Collisioni totali:   " << totalCollisions << '\n';
    std::cout << "Tempo medio/frame:   " << (frames > 0 ? totalTime / frames : 0) << " us\n";
    std::cout << "FPS medi:            "
              << std::fixed << std::setprecision(1)
              << (frames / SIMULATION_SECONDS) << '\n';

    // -- Cleanup ------------------------------------------------------------
    for (Entity* e : entities) delete e;

    return 0;
}