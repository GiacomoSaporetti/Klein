#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <Windows.h>
//#include "SDL3/SDL.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "LinkedList.h"
#include "CollisionHandler.h"
#define N 200
#define R 100
using namespace std;

Klein::TimeHandler TIME;

Klein::Entity * e;
int Klein::Entity::time_direction = 1;
Klein::TimeHandler* Klein::Entity::TIMER = &TIME;
Klein::CollisionHandler COLLISION;
long int times[5]={0,0,0,0, 0};
int main()
{
    srand((unsigned)time(NULL));

    for(int t=0; t<R; t++)
    {
        cout << t <<endl;
        Klein::LinkedList LIST;
        COLLISION.SetEntitiesList(&LIST);
        Klein::Entity* t_entity;
        for(int i=0; i<N; i++)
        {
            t_entity = new Klein::Entity;
            t_entity->SetPosition({(float)(rand()%9999)/100.0f, (float)(rand()%9999)/100.0f});
            t_entity->SetRadius((float)(rand()%499)/100.0f);
            /*t_entity->SetPosition({50, 50});
            t_entity->SetRadius(2);*/
            LIST.AddNodeEnd(t_entity);
            //cout << t_entity <<endl;
        }
        int res;
        //cout <<endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        res = COLLISION.Run();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        times[0] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        //cout << res << endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ns]" << std::endl;
        

        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized();
        end = std::chrono::steady_clock::now();
        times[1] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        //cout << res << endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ns]" << std::endl;
    
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized2();
        end = std::chrono::steady_clock::now();
        times[2] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        //cout << res << endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ns]" << std::endl;

        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized3();
        end = std::chrono::steady_clock::now();
        times[3] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        //cout << res << endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ns]" << std::endl;
        begin = std::chrono::steady_clock::now();
        res = COLLISION.RunOptimized4();
        end = std::chrono::steady_clock::now();
        times[4] += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        //cout << res << endl;
    }

    cout << N<< ", "<<times[0]/R << ", " << times[1]/R << ": "<<(float)times[0]/(float)times[1]<< ", " << times[2]/R <<": "<<(float)times[0]/(float)times[2]<< ", " << times[3]/R << ": "<<(float)times[0]/(float)times[3]<<", " << times[4]/R<< ": "<<(float)times[0]/(float)times[4]<< endl;
    return 0;
}

