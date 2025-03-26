#pragma once 
#include <sys/time.h>
#include <climits>
#include <iostream>
#include <math.h>
#include <semaphore.h>
#include <thread>


//#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(name, val) std::cout << name << " " << val << std::endl;

#else
#define DEBUG_MSG(name, val)
#endif

//#define MAX_NUMBER_OF_ENTITIES 100
#define MAX_PARTICLE_SIZE   50
#define SCREEN_Y 1080
#define SCREEN_X 1920

#define INT_ERROR_VALUE -1

namespace Klein
{
    static int MAX_NUMBER_THREADS = std::thread::hardware_concurrency();
    static std::thread* THREADS = (std::thread*) std::malloc(MAX_NUMBER_THREADS*sizeof(std::thread)); ;
    struct time_profile
    {
        struct timespec value;
        float delta = 0.0f;
        float speed = 1.0f;
        float time = 0.0f;
    };
        
    struct vector
    {
        float x;
        float y;

        float Magnitude()
        {return sqrt(x*x + y*y);}

        void Rescale(float factor)
        {
            x = factor*x;
            y = factor*y;
        }
  
        vector Versor()
        {
            vector res;
            res.x = x;
            res.y = y;
            float m = res.Magnitude();
            res.Rescale(1/m);
            return res;
        }
    };


    struct Point
    {
        int x;
        int y;
    };

    struct Node 
    {
        Node* next = nullptr;
        void* data = nullptr;
    };

    enum HITBOX_TYPE
    {
        CIRCLE,
        RECTANGLE,
    };

    struct Rectangle
    {
        int top=0;
        int right=0;
        int bottom=0;
        int left=0;

        int Area()
        {return abs((top-bottom)*(right-left));}
    };
}