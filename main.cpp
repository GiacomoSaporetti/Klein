#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
//#include <Windows.h>
//#include "SDL3/SDL.h"
#include "Klein.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "LinkedList.h"
#include "CollisionHandler.h"
#define N 500
#define R 1


#define RES

#ifdef RES
#define PRINT_RES cout << res << endl;
#else
#define PRINT_RES
#endif

using namespace std;

Klein::TimeHandler TIME;

Klein::Entity * e;
int Klein::Entity::time_direction = 1;
Klein::TimeHandler* Klein::Entity::TIMER = &TIME;
Klein::CollisionHandler COLLISION;
long long int times[7]={1,1,1,1,1,1,1};
int main()
{
    srand((unsigned)time(NULL));

    for(int t=0; t<R; t++)
    {
        Klein::LinkedList LIST;
        COLLISION.SetEntitiesList(&LIST);
        Klein::Entity* t_entity;
        for(int i=0; i<N; i++)
        {
            t_entity = new Klein::Entity;
            t_entity->faction = i%2;
            t_entity->SetPosition({rand()%100, rand()%100});
            t_entity->hitbox.type = Klein::CIRCLE;
            t_entity->SetRadius(rand()%MAX_PARTICLE_SIZE/2);
            //t_entity->SetHeight(rand()%MAX_PARTICLE_SIZE/2);
            //t_entity->SetWidth(rand()%MAX_PARTICLE_SIZE/2);
            
            if(i%2==0) 
            {
                t_entity->hitbox.type = Klein::RECTANGLE;
                t_entity->SetHeight(rand()%MAX_PARTICLE_SIZE);
                t_entity->SetWidth(rand()%MAX_PARTICLE_SIZE);
            }
            LIST.AddNodeEnd(t_entity);
        }

        Klein::vector mean={0,0};
        Klein::vector var={0,0};

        for(int i=0; i<N; i++)
        {
            Klein::Entity* entity = (Klein::Entity*)LIST.GetData(i);
            Klein::Point position = entity->GetPosition();
            mean.x += position.x;
            mean.y += position.y;
            var.x += position.x*position.x;
            var.y += position.y*position.y;
        }
        mean.x = mean.x/N;
        mean.y = mean.y/N;
        var.x += mean.x*mean.x/N;
        var.y += mean.y*mean.y/N;

        for(int i=0; i<N; i++)
        {
            Klein::Entity* entity = (Klein::Entity*)LIST.GetData(i);
            Klein::Point position = entity->GetPosition();
            var.x -= 2*position.x*mean.x/N;
            var.y -= 2*position.y*mean.y/N;
        }

        cout << "mean: " << mean.x << "," << mean.y << "var: " << var.x << "," << var.y << endl;

        int res;
        //cout <<endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        res = COLLISION.Run();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        times[0] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
   

        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized();
        end = std::chrono::steady_clock::now();
        times[1] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
    
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized2();
        end = std::chrono::steady_clock::now();
        times[2] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
 
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized3();
        end = std::chrono::steady_clock::now();
        times[3] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
        
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized4();
        end = std::chrono::steady_clock::now();
        times[4] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
       
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized5();
        end = std::chrono::steady_clock::now();
        times[5] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES

        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized6(2);
        end = std::chrono::steady_clock::now();
        times[6] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
    }

    cout << N << " | ";
    cout << times[0]/R << " | ";
    cout << times[1]/R << " : " <<(float)times[0]/(float)times[1]<< " | ";
    cout << times[2]/R << " : " <<(float)times[0]/(float)times[2]<< " | ";
    cout << times[3]/R << " : " <<(float)times[0]/(float)times[3]<< " | ";
    cout << times[4]/R << " : " <<(float)times[0]/(float)times[4]<< " | ";
    cout << times[5]/R << " : " <<(float)times[0]/(float)times[5]<< " | ";
    cout << times[6]/R << " : " <<(float)times[0]/(float)times[6]<< " | ";
    cout << endl;


    return 0;
}

