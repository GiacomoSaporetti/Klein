#pragma once 
#include <sys/time.h>
#include <climits>
#include <iostream>
#include <math.h>
#include <thread>
//#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(name, val) std::cout << name << " " << val << std::endl

#else
#define DEBUG_MSG(name, val)
#endif


#define MAX_PARTICLE_SIZE   50
#define SCREEN_Y 1000
#define SCREEN_X 1000

#define INT_ERROR_VALUE -1


namespace Klein
{
    #define MAX_NUMBER_OF_ENTITIES 100
    
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

    struct Hitbox
    {
        enum HITBOX_TYPE type=CIRCLE;
        struct Point center={0,0};
        float radius=0;
        int width=0;
        int height=0;
        struct Point top_right={0,0};
        struct Point bottom_left={0,0};

        void SetWidth(int w)
        {
            width = w;
            top_right.x = center.x + w/2;
            bottom_left.x = center.x - w/2;
            radius = std::sqrtf(width*width + height*height)/2;
        }

        void SetHeight(int h)
        {
            height = h;
            top_right.y = center.y + h/2;
            bottom_left.y = center.y - h/2;
            radius = std::sqrtf(width*width + height*height)/2;
        }

        void SetRadius(float r)
        {
            radius = r;
            top_right.x = center.x + r;
            bottom_left.x = center.x - r;
            top_right.y = center.y + r;
            bottom_left.y = center.y - r;
        }

        int Top()
        {
            return top_right.y;
        }
        int Bottom()
        {
            return bottom_left.y;
        }
        int Left()
        {
            return bottom_left.x;
        }
        int Right()
        {
            return top_right.x;
        }
    };

}