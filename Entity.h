#pragma once

#include "Klein.h"
#include "TimeHandler.h"

namespace Klein
{
    class Entity
    {
        private:
            
        public:
            vector speed;
            vector position;
            float birth;
            float death;
            float mass;
            int hp=50;
            Entity()
            {
                mass = 0.0f;
                speed.x = 0.0f;
                speed.y = 1.0f;
                position.x = 0.0f;
                position.y = 0.0f;
                if(TIMER != NULL)
                    birth  = TIMER->GetGameTime();
                else birth = -1.0f;
                death = -1.0f;
            }
            ~Entity() = default;   
            static int time_direction;
            static TimeHandler * TIMER;
            void Run();
            int GetTimeDirection();
            void DeathState();
            vector GetPosition();
            vector GetSpeed();
    };
}