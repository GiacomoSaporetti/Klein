#define SDL_ENABLE_OLD_NAMES

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>
#include <iomanip>

#include "SDL3/SDL.h"
#include "Klein.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "CollisionHandler.h"

using namespace Klein;

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

int main()
{
    // -- SDL init -----------------------------------------------------------
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init fallito: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window*   window   = SDL_CreateWindow("Klein – 2 particelle",
                                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!window || !renderer)
    {
        std::cerr << "Creazione finestra/renderer fallita: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // -- Entità -------------------------------------------------------------
    const float cx    = SCREEN_WIDTH  / 2.f;
    const float cy    = SCREEN_HEIGHT / 2.f;
    const int   R     = 30;        // raggio hitbox e visivo
    const float SPEED = 120.f;     // pixel/secondo

    // Particella A — parte da sinistra, va a destra
    Entity* a = new Entity();
    a->setPosition({ cx - 200.f, cy });
    a->setSpeed   ({ SPEED, 0.f });
    {
        circle_t shape;
        shape.center = { 0, 0 };
        shape.radius = R;
        a->addHitbox(new Hitbox(*a, shape));
    }

    // Particella B — parte da destra, va a sinistra
    Entity* b = new Entity();
    b->setPosition({ cx + 200.f, cy });
    b->setSpeed   ({ -SPEED, 0.f });
    {
        circle_t shape;
        shape.center = { 0, 0 };
        shape.radius = R;
        b->addHitbox(new Hitbox(*b, shape));
    }

    std::vector<Entity*> entities = { a, b };

    // -- Timer e handler ----------------------------------------------------
    TimeHandler timer;
    Entity::setTimer(&timer);

    CollisionHandler handler;
    handler.setEntitiesList(entities);

    // -- Statistiche --------------------------------------------------------
    int       totalCollisions = 0;
    int       frames          = 0;
    long long totalTime       = 0;


    const auto simStart = std::chrono::steady_clock::now();

    // -- Game loop ----------------------------------------------------------
    bool running         = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.scancode == SDL_SCANCODE_ESCAPE) running = false;
        }

        timer.tick();

        const int collisions = handler.runGridOptimization();
        totalCollisions += collisions;
        totalTime       += GET_TIME;
        frames++;
        
        if (collisions > 0)
            std::cout << "COLLISIONE!"
                      << "  A=("  << a->getPosition().x << "," << a->getPosition().y << ")"
                      << "  vA=(" << a->getSpeed().x    << "," << a->getSpeed().y    << ")"
                      << "  B=("  << b->getPosition().x << "," << b->getPosition().y << ")"
                      << "  vB=(" << b->getSpeed().x    << "," << b->getSpeed().y    << ")"
                      << '\n';

        // -- Rendering -------------------------------------------------------
        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
        SDL_RenderClear(renderer);

        // Particella A — blu
        SDL_SetRenderDrawColor(renderer, 80, 140, 255, 255);
        drawFilledCircle(renderer,
                         static_cast<int>(a->getPosition().x),
                         static_cast<int>(a->getPosition().y), R);

        // Particella B — arancione
        SDL_SetRenderDrawColor(renderer, 255, 140, 40, 255);
        drawFilledCircle(renderer,
                         static_cast<int>(b->getPosition().x),
                         static_cast<int>(b->getPosition().y), R);

        SDL_RenderPresent(renderer);

        if(timer.getGameTime() > 10 * 1E9) //Dopo 15s inverto il tempo
        {  
            timer.setGameSpeed(-1.0f);  
            Klein::Entity::setTimeDirection(-1);
        }
    }

    std::cout << "Collisioni totali: " << totalCollisions << '\n';

    delete a;
    delete b;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

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

    return 0;
}
