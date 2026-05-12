// Init.cpp
#include "Entity.h"
#include "CollisionHandler.h"
#include "TimeHandler.h"

namespace Klein
{
    extern std::vector<Entity*>    g_all_entities;   
    extern std::vector<Hitbox*>    g_walls;    
    extern TimeHandler             g_timer;           
    extern CollisionHandler        g_collision_handler; 

    struct _Init {
        _Init() 
        { 
            g_all_entities.reserve(100);
        }
    } _init;


    void RunFrame()
    {
      g_timer.tick();
      g_collision_handler.runGridOptimization();
      g_collision_handler.removeInactiveCollisions();

      for (Entity* e : g_all_entities)  e->tick();
    }

}