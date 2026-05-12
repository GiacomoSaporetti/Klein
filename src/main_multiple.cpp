//#define USE_GRAPHICS
#define SDL_ENABLE_OLD_NAMES

#include "main.h"


#define NUMBER_OF_ENTITIES  150
#define ENTITY_RADIUS       25.f


using namespace Klein;

   
auto simStart = std::chrono::steady_clock::now();
static int       frames          = 0;


static void print_stats()
{
    std::cout << "\nFrames simulati:     " << frames          << '\n';
    std::cout << "FPS medi:            "
              << std::fixed << std::setprecision(1)
              << (frames > 0
                     ? frames / std::chrono::duration<double>(
                                    std::chrono::steady_clock::now() - simStart).count()
                     : 0.0)
              << '\n';

    exit(0);  
}


void signal_handler(int sig)
{
    print_stats();
}


static void drawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int r)
{
    #ifdef USE_GRAPHICS
    for (int dy = -r; dy <= r; dy++)
    {
        int dx = static_cast<int>(std::sqrt(static_cast<float>(r * r - dy * dy)));
        SDL_RenderLine(renderer,
                       static_cast<float>(cx - dx), static_cast<float>(cy + dy),
                       static_cast<float>(cx + dx), static_cast<float>(cy + dy));
    }
    #endif
}



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



int main()
{
    //atexit(print_stats);
    signal(SIGTERM, signal_handler);
    signal(SIGINT,  signal_handler);
    const unsigned int baseSeed = static_cast<unsigned>(time(nullptr));
    std::cout << "Seed:    " << baseSeed          << '\n';
    std::cout << "Entita': " << NUMBER_OF_ENTITIES << '\n';


    #ifdef USE_GRAPHICS
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
    #endif


    for (int i = 0; i < NUMBER_OF_ENTITIES; i++)
    {
        Entity* e = makeEntity(SCREEN_WIDTH, SCREEN_HEIGHT, baseSeed + i);
        if(i%2) e->setKleiness(true);
        Klein::AddEntity(e);
    }
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

    simStart = std::chrono::steady_clock::now();

    bool running = true;
    while (running)
    {
        frames++;
        const auto frameStart = std::chrono::steady_clock::now();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.scancode == SDL_SCANCODE_ESCAPE) running = false;
        }

        Klein::RunFrame();



        #ifdef USE_GRAPHICS
        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);   // sfondo scuro
        SDL_RenderClear(renderer);

        for (Entity* e : Klein::GetAllEntities())
        {
            const point_t pos = e->getPosition();
            const int     cx  = static_cast<int>(pos.x);
            const int     cy  = static_cast<int>(pos.y);
            const int     r   = e->getHitboxes()[0]->getRadius();

            const Color& c = baseColors[0];
            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 200);


            drawFilledCircle(renderer, cx, cy, r);
        }

        SDL_RenderPresent(renderer);
        #endif

        const auto frameEnd = std::chrono::steady_clock::now();
        const auto frameDuration = std::chrono::duration<double>(frameEnd - frameStart).count();
        /*if (frameDuration < 0.016)
            SDL_Delay(static_cast<Uint32>((0.016 - frameDuration) * 1000));
        printf("frameDuration: %f\n", frameDuration);*/

        if(Klein::GetGameTime() > 5 * 1E9)  Klein::SetGameSpeed(-1.0f);  //Dopo 15s inverto il tempo

        //if(frames > 1000) running = false;
    }

    print_stats();
    // -- Cleanup ------------------------------------------------------------
    for (Entity* e : Klein::GetAllEntities()) delete e;
    #ifdef USE_GRAPHICS
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    #endif

    return 0;
} 
