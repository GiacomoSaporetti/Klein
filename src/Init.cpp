// Init.cpp
#include "Entity.h"
#include "CollisionHandler.h"
#include "MainCharacter.h"
#include "TimeHandler.h"

namespace Klein
{
    extern std::vector<Entity*>     g_all_entities;   
    extern std::vector<Hitbox*>     g_walls;    
    extern TimeHandler              g_timer;           
    extern CollisionHandler         g_collision_handler; 
    extern MainCharacter            g_main_char;
    extern InputHandler             g_input_handler;

    struct _Init {
        _Init() 
        { 
            g_all_entities.reserve(100);
            AddEntity(g_main_char);
            Hitbox(g_main_char, {0, 100 , 0 , 100});
            g_main_char.setPosition()
        }
    } _init;


    /*Esecuzione di un frame*/
    void RunFrame()
    {
        /*1- Aggiorno il timer*/     
        g_timer.tick();   

        /*2- Poll degli input*/
        g_input_handler.pollUserInput();

        /*3- Eseguo le azioni del giocatore*/
        g_main_char.tick();

        /*4- Gestisco le collisioni*/
        g_collision_handler.runGridOptimization();
        g_collision_handler.removeInactiveCollisions();

        /*5- Tick delle singole entità*/
        for (Entity* e : g_all_entities)  e->tick();
    }

}