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

#define N 1000
#define R 100


//#define RES

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
int number_of_hitbox = 0;
int main()
{
    srand((unsigned)time(NULL));

    for(int t=0; t<R; t++)
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        Klein::LinkedList LIST;
        COLLISION.SetEntitiesList(&LIST);
        Klein::Entity* t_entity;
        for(int i=0; i<N; i++)
        {
            t_entity = new Klein::Entity;
            int x, y, r;
            x = rand()%SCREEN_X/10;
            y = rand()%SCREEN_Y/10;
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

        //MEAN AND VARIANCE

        /*begin = std::chrono::steady_clock::now();*/
       /*Klein::vector mean={0,0};
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
        var.x = (var.x/N - mean.x*mean.x);
        var.y = (var.y/N - mean.y*mean.y);
        

        /*end = std::chrono::steady_clock::now();
        cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << endl;*/
        //cout << "mean: " << mean.x << ", " << mean.y << " | var: " << var.x << ", " << var.y << endl;



        int res=0;
    
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunNaive();
        end = std::chrono::steady_clock::now();
        times[0] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
   

        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunQuadrantOptimization();
        end = std::chrono::steady_clock::now();
        times[1] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
    
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunGridOptimization();
        end = std::chrono::steady_clock::now();
        times[2] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        PRINT_RES
    }

    cout << N << " | ";
    cout << number_of_hitbox/R << " | ";
    cout << times[0]/R << " | ";
    cout << times[1]/R << " : " <<(float)times[0]/(float)times[1]<< " | ";
    cout << times[2]/R << " : " <<(float)times[0]/(float)times[2]<< " | ";
    cout << endl;

    /*const auto processor_count = std::thread::hardware_concurrency();
    
    cout << processor_count << endl;
    long long int* res;
    res = (long long int*) malloc(Klein::MAX_NUMBER_THREADS*sizeof(long long int));
    int L = 10000;
    for(int i=0; i<L; i++)
    {
    auto begin = std::chrono::steady_clock::now();

    for(int th=0; th<Klein::MAX_NUMBER_THREADS; th++)
    {
        Klein::THREADS[th] = std::thread(fun1, 2*th, res + th);
    }

    for(int th=0; th<Klein::MAX_NUMBER_THREADS; th++)
    {
        Klein::THREADS[th].join();
    }

    auto end = std::chrono::steady_clock::now();
    times[0] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count(); 
    }
    
    cout << "Total time: " << times[0]/L << endl;
    for(int th=0; th<Klein::MAX_NUMBER_THREADS; th++)
    {
    cout << *(res + th) << endl;
    }
    cout << Klein::MAX_NUMBER_THREADS << endl;*/
    
    return 0;
}

