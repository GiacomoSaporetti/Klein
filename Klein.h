#pragma once 
#include <sys/time.h>
#include <math.h>
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

    struct Node 
    {
        Node* next;
        void* data;
    };
}