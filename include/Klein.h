#pragma once 
#include <sys/time.h>
#include <climits>
#include <iostream>
#include <math.h>
#include <semaphore.h>
#include <thread>
#include <list>
#include <assert.h>


#ifdef DEBUG
#define PRINT_DEBUG(name, val) std::cout << name << " " << val << std::endl;
#define ASSERT(expr)    assert(expr);
#else
#define PRINT_DEBUG(name, val)
#define ASSERT(expr)   
#endif


#define MAX_PARTICLE_SIZE   50
#define SCREEN_Y 1080
#define SCREEN_X 1920

#define INT_ERROR_VALUE -1

namespace Klein
{
    static int MAX_NUMBER_THREADS = std::thread::hardware_concurrency();
    static std::thread* THREADS = (std::thread*) std::malloc(MAX_NUMBER_THREADS*sizeof(std::thread)); ;
    struct time_profile_t
    {
        struct timespec value;
        float delta = 0.0f;
        float speed = 1.0f;
        float time = 0.0f;
    };
        
    struct vector_t
    {
        float x;
        float y;

        float getMagnitude()
        {return sqrt(x*x + y*y);}

        void rescaleByFactor(float factor)
        {
            x = factor*x;
            y = factor*y;
        }
  
        vector_t getVersor()
        {
            vector_t res;
            res.x = x;
            res.y = y;
            float m = res.getMagnitude();
            res.rescaleByFactor(1/m);
            return res;
        }
    };


    struct point_t
    {
        int x;
        int y;
    };

    struct node_t 
    {
        node_t* next = nullptr;
        node_t* previous = nullptr;
        void* data = nullptr;
    };

    enum hitbox_type_t
    {
        CIRCLE,
        RECTANGLE,
    };

    struct rectangle_t
    {
        int top=0;
        int right=0;
        int bottom=0;
        int left=0;

        int getArea()    {return abs((top-bottom)*(right-left));}
        int getPerimeter() {return 2*(abs(top-bottom) + abs(right-left));}
    };
}