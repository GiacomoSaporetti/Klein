#define SDL_ENABLE_OLD_NAMES

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <Windows.h>
#include "SDL3/SDL.h"
#include "Klein.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "LinkedList.h"
#include "CollisionHandler.h"
#include "MainCharacter.h"

#define N 1000
#define R 1000
using namespace std;


//#define RES

#ifdef RES
#define PRINT_RES cout << res << endl;
#else
#define PRINT_RES
#endif

Klein::MainCharacter PG;

void ReadKeys()
{
    if(GetKeyState('A') & 0x8000)   ;
    if(GetKeyState('D') & 0x8000)   ;
    if(GetKeyState('S') & 0x8000)   ;
    if(GetKeyState('W') & 0x8000)   ;
}


Klein::TimeHandler TIME;

Klein::Entity * e;
int Klein::Entity::time_direction = 1;
Klein::TimeHandler* Klein::Entity::TIMER = &TIME;
Klein::CollisionHandler COLLISION;
Klein::LinkedList ENTITIES_LIST;

long long int times[7]={1,1,1,1,1,1,1};
int number_of_hitbox = 0;

SDL_Event event;
int quit = 0;
int main()
{
    srand((unsigned)time(NULL));

    std::cout << "New compiled" << std::endl;
    float x=0, y=0, prev_x, prev_y;
    while( !quit )
    {
        prev_x = x;
        prev_y = y;
        TIME.Run();
        
        //system("cls");
        std::cout << PG.position.x << ", " << PG.position.y << " | " << 1/TIME.GetRealDelta() <<std::endl;
        //std::cout << 1/TIME.GetRealDelta() <<std::endl;
        while(!TIME.WaitUntil(1.0f/30))
        {;}
    }

    /*COLLISION.SetEntitiesList(&ENTITIES_LIST);

    for(int t=0; t<R; t++)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        Klein::LinkedList LIST;
        COLLISION.SetEntitiesList(&LIST);
        Klein::Entity* t_entity;
        for(int i=0; i<N; i++)
        {
            //cout << "new entity" << endl;
            t_entity = new Klein::Entity;
            int x, y, r;
            x = rand()%SCREEN_X;
            y = rand()%SCREEN_Y;
            r = rand()%10 - 11;
            //cout << "Entity: " << t_entity << endl;
            number_of_hitbox ++;
            Klein::Point p;
            p = {x + rand()%10, y + rand()%10};
            t_entity->AddHitbox(Klein::CIRCLE, &p, (rand()%MAX_PARTICLE_SIZE)/2, 0, 0);
            for(int i=0; i<r; i++)
            {
                number_of_hitbox ++;
                Klein::Point p;
                p = {x + rand()%10, y + rand()%10};
                t_entity->AddHitbox(Klein::RECTANGLE, &p, 0,rand()%MAX_PARTICLE_SIZE, rand()%MAX_PARTICLE_SIZE);
            }
            t_entity->SetFaction(i%2);
            t_entity->SetPosition({x, y});
      
            LIST.AddNodeEnd(t_entity);
        }

        int res=0;
        res = COLLISION.RunGridOptimization();
 
    }

    /*cout << N << " | ";
    cout << number_of_hitbox/R << " | ";
    cout << times[0]/R << " | ";
    cout << times[1]/R << " : " <<(float)times[0]/(float)times[1]<< " | ";
    cout << times[2]/R << " : " <<(float)times[0]/(float)times[2]<< " | ";
    cout << endl;*/

    return 0;
}

