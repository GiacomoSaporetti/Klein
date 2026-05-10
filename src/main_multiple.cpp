#define SDL_ENABLE_OLD_NAMES

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <unordered_set>
#include <cmath>

#include "SDL3/SDL.h"
#include "Klein.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "CollisionHandler.h"

// -------------------------------------------------------------------------
// Configurazione
// -------------------------------------------------------------------------

#define NUMBER_OF_ENTITIES  100
#define ENTITY_RADIUS       25.f

// Commenta questa riga per girare a tempo indefinito (finché non chiudi la finestra)
// #define SIMULATION_SECONDS  20.0

using namespace Klein;

// -------------------------------------------------------------------------
// Disegno di un cerchio pieno con SDL3
// -------------------------------------------------------------------------

static void drawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int r)
{
    for (int dy = -r; dy <= r; dy++)
    {
        int dx = static_cast<int>(std::sqrt(static_cast<float>(r * r - dy * dy)));
        SDL_RenderLine(renderer,
                       static_cast<float>(cx - dx), static_cast<float>(cy + dy),
                       static_cast<float>(cx + dx), static_cast<float>(cy + dy));
    }
}

// -------------------------------------------------------------------------
// Utility – creazione entità
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
        (rand() % 200 - 100) * 1.f,
        (rand() % 200 - 100) * 1.f
    };
    e->setSpeed(vel);

    circle_t shape;
    shape.radius = rand() % (MAX_PARTICLE_SIZE/2 - 1) * 1.f;
    shape.center = {0,0};

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
    std::cout << "Seed:    " << baseSeed          << '\n';
    std::cout << "Entita': " << NUMBER_OF_ENTITIES << '\n';
#ifdef SIMULATION_SECONDS
    std::cout << "Durata:  " << SIMULATION_SECONDS << " secondi\n\n";
#else
    std::cout << "Durata:  indefinita (chiudi la finestra per uscire)\n\n";
#endif

    // -- SDL init -----------------------------------------------------------
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init fallito: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window*   window   = SDL_CreateWindow("Klein – Particle Simulation", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!window || !renderer)
    {
        std::cerr << "Creazione finestra/renderer fallita: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // -- Entità -------------------------------------------------------------
    std::vector<Entity*> entities;
    entities.reserve(NUMBER_OF_ENTITIES);
    for (int i = 0; i < NUMBER_OF_ENTITIES; i++)
        entities.push_back(makeEntity(SCREEN_WIDTH, SCREEN_HEIGHT, baseSeed + i));

    // Colore base per ogni entità (assegnato una volta sola)
    struct Color { Uint8 r, g, b; };
    std::vector<Color> baseColors(NUMBER_OF_ENTITIES);
    {
        srand(baseSeed ^ 0xDEADBEEF);
        for (auto& c : baseColors)
        {
            c.r = 60  + rand() % 140;
            c.g = 120 + rand() % 100;
            c.b = 180 + rand() % 75;
        }
    }

    // -- Timer e handler ----------------------------------------------------
    TimeHandler timer;
    Entity::setTimer(&timer);

    CollisionHandler handler;
    handler.setEntitiesList(entities);

    // -- Statistiche --------------------------------------------------------
    int       totalCollisions = 0;
    int       frames          = 0;
    long long totalTime       = 0;

    // Per flash di collisione: contatore "frames rimanenti da mostrare rosso"
    std::vector<int> collisionFlash(NUMBER_OF_ENTITIES, 0);
    constexpr int FLASH_FRAMES = 4;

    const auto simStart = std::chrono::steady_clock::now();

    // -- Game loop ----------------------------------------------------------
    bool running = true;
    while (running)
    {
        const auto frameStart = std::chrono::steady_clock::now();
        // Gestione eventi SDL
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.scancode == SDL_SCANCODE_ESCAPE) running = false;
        }

#ifdef SIMULATION_SECONDS
        const auto   now     = std::chrono::steady_clock::now();
        const double elapsed = std::chrono::duration<double>(now - simStart).count();
        if (elapsed >= SIMULATION_SECONDS) running = false;
#endif

        // Aggiorna simulazione
        timer.tick();

        BEGIN_TIME;
        const int collisions = handler.runGridOptimization();
        END_TIME;

        totalCollisions += collisions;
        totalTime       += GET_TIME;
        frames++;

        // Aggiorna flash (semplice: lampeggia tutte le entità in collisione)
        // CollisionHandler non espone quali entità collidono, quindi usiamo
        // un effetto globale proporzionale al numero di collisioni rilevate.
        if (collisions > 0)
        {
            // Marca un sottoinsieme casuale di entità come "in collisione"
            // (approssimazione visiva — adatta se CollisionHandler non
            //  restituisce gli indici delle coppie collidenti)
            for (int k = 0; k < std::min(collisions * 2, NUMBER_OF_ENTITIES); k++)
            {
                int idx = rand() % NUMBER_OF_ENTITIES;
                collisionFlash[idx] = FLASH_FRAMES;
            }
        }
        for (int& f : collisionFlash) if (f > 0) f--;

        // -- Rendering -------------------------------------------------------
        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);   // sfondo scuro
        SDL_RenderClear(renderer);

        for (int i = 0; i < NUMBER_OF_ENTITIES; i++)
        {
            const point_t pos = entities[i]->getPosition();
            const int     cx  = static_cast<int>(pos.x);
            const int     cy  = static_cast<int>(pos.y);
            const int     r   = entities[i]->getHitboxes()[0]->getRadius();

            if (collisionFlash[i] > 0)
            {
                // Flash rosso brillante durante collisione
                SDL_SetRenderDrawColor(renderer, 255, 60, 60, 220);
            }
            else
            {
                const Color& c = baseColors[i];
                SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 200);
            }

            drawFilledCircle(renderer, cx, cy, r);
        }

        SDL_RenderPresent(renderer);

        const auto frameEnd = std::chrono::steady_clock::now();
        const auto frameDuration = std::chrono::duration<double>(frameEnd - frameStart).count();
        if (frameDuration < 0.016)
            SDL_Delay(static_cast<Uint32>((0.016 - frameDuration) * 1000));
        printf("frameDuration: %f\n", frameDuration);

        if(timer.getGameTime() > 15 * 1E9)  timer.setGameSpeed(-1.0f);  //Dopo 15s inverto il tempo
    }

    // -- Report finale ------------------------------------------------------
    std::cout << "\nFrames simulati:     " << frames          << '\n';
    std::cout << "Collisioni totali:   " << totalCollisions << '\n';
    std::cout << "Tempo medio/frame:   "
              << (frames > 0 ? totalTime / frames : 0LL) << " us\n";
    std::cout << "FPS medi:            "
              << std::fixed << std::setprecision(1)
              << (frames > 0
                     ? frames / std::chrono::duration<double>(
                                    std::chrono::steady_clock::now() - simStart).count()
                     : 0.0)
              << '\n';

    // -- Cleanup ------------------------------------------------------------
    for (Entity* e : entities) delete e;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
} 