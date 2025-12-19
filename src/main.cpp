#define SDL_ENABLE_OLD_NAMES

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
//#include <Windows.h>
#include "SDL3/SDL.h"
#include "Klein.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "LinkedList.h"
#include "CollisionHandler.h"
#include "MainCharacter.h"

//extern std::chrono::steady_clock::time_point timeMeasureBegin;
//extern std::chrono::steady_clock::time_point timeMeasureEnd;

#define NUMBER_OF_ENTITIES 10000
#define NUMBER_OF_REPETITIONS 1
using namespace std;


//#define RES

#ifdef RES
#define PRINT_RES cout << res << endl;
#else
#define PRINT_RES
#endif

Klein::MainCharacter PG;

/*void ReadKeys()
{
    if(GetKeyState('A') & 0x8000)   printf("A");
    if(GetKeyState('D') & 0x8000)   printf("D");
    if(GetKeyState('S') & 0x8000)   printf("S");
    if(GetKeyState('W') & 0x8000)   printf("W");
}*/


Klein::TimeHandler TIME_HANDLER;

Klein::Entity * e;
int Klein::Entity::directionOfTime = 1;
Klein::TimeHandler* Klein::Entity::TIMER = &TIME_HANDLER;
Klein::CollisionHandler COLLISION_HANDLER;
Klein::LinkedList ENTITIES_LIST;

long long int times[7]={1,1,1,1,1,1,1};
int numberOfHitboxes = 0;

SDL_Event event;
int quit = 0;
int main()
{
    srand((unsigned)time(NULL));

    /*std::cout << "New compiled" << std::endl;
    float x=0, y=0, prev_x, prev_y;
    while( !quit )
    {
        prev_x = x;
        prev_y = y;
        TIME_HANDLER.Run();
        
        //system("cls");
        //std::cout << PG.position.x << ", " << PG.position.y << " | " << 1/TIME_HANDLER.getRealDelta() <<std::endl;
        //std::cout << 1/TIME_HANDLER.getRealDelta() <<std::endl;
        while(!TIME_HANDLER.WaitUntil(1.0f/60))
        {;}
    }*/

    
    //ENTITIES_LIST.clear();
    COLLISION_HANDLER.setEntitiesList(ENTITIES_LIST);
    
    for(int t=0; t<NUMBER_OF_REPETITIONS; t++)
    {
        for(int i=0; i<NUMBER_OF_ENTITIES; i++)
        {
            Klein::Entity* newEntity = new Klein::Entity;
            int numberOfRectangles = rand()%10 - 7;
    
            numberOfHitboxes ++;
            Klein::point_t randomCenter= {rand()%SCREEN_X, rand()%SCREEN_Y};    //Random center point
            const int defaultHeight = 0; //Doesn't matter for Klein::hitbox_type_t::CIRCLE
            const int defaultWidth = 0;  //Doesn't matter for Klein::hitbox_type_t::CIRCLE

            newEntity->addHitbox(
                Klein::hitbox_type_t::CIRCLE, 
                randomCenter, 
                (rand()%MAX_PARTICLE_SIZE)/2.0f, 
                defaultWidth, 
                defaultHeight);

            for(int i=0; i<numberOfRectangles; i++)
            {
                numberOfHitboxes ++;
                Klein::point_t rectangleCenter;
                rectangleCenter = {randomCenter.x + rand()%10, randomCenter.y + rand()%10};
                const float defaultRadius = 0.0f; //Doesn't matter for Klein::hitbox_type_t::RECTANGLE
                
                newEntity->addHitbox(
                    Klein::RECTANGLE, 
                    rectangleCenter, 
                    defaultRadius, 
                    rand()%MAX_PARTICLE_SIZE/10, 
                    rand()%MAX_PARTICLE_SIZE/10);
            }
            newEntity->setFaction(i%2);
            newEntity->setPosition({randomCenter.x, randomCenter.y});
      
            ENTITIES_LIST.appendNode(newEntity);
        }
    
        BEGIN_TIME
        int optimized = COLLISION_HANDLER.runGridOptimization();
        END_TIME
        PRINT_TIME("optimized")
    
        BEGIN_TIME
        int naive = COLLISION_HANDLER.runNaiveImplementation();
        END_TIME
        PRINT_TIME("naive")
    
        cout << optimized << "|" << naive << endl;
        ENTITIES_LIST.clear();
    }

    cout << "Number of Entities: "<< NUMBER_OF_ENTITIES << "\n";
    cout << numberOfHitboxes/NUMBER_OF_REPETITIONS << " | ";
    cout << times[0]/NUMBER_OF_REPETITIONS << " | ";
    cout << times[1]/NUMBER_OF_REPETITIONS << " : " <<(float)times[0]/(float)times[1]<< " | ";
    cout << times[2]/NUMBER_OF_REPETITIONS << " : " <<(float)times[0]/(float)times[2]<< " | ";
    cout << endl;

    return 0;
}

