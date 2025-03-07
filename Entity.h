#pragma once

#include "Klein.h"
#include "TimeHandler.h"
#include "LinkedList.h"

namespace Klein
{
    class Entity
    {
        private:
            LinkedList* recently_collided;
        public:
            vector speed;
            Point position;
            Hitbox hitbox;
            float birth;
            float death;
            float mass;
            float radius;
            int hp=50;
            int faction;
            int cell_id;
            static int time_direction;
            static TimeHandler * TIMER;
            Entity()
            {
                hitbox.type = CIRCLE;
                hitbox.radius = 2.0f;
                mass = 1.0f;
                speed.x = 0.0f;
                speed.y = 1.0f;
                position.x = 0;
                position.y = 0;
                radius = 0.0f;
                if(TIMER != NULL)
                    birth  = TIMER->GetGameTime();
                else birth = -1.0f;
                death = -1.0f;
                faction = 0;
                cell_id = -1;
                recently_collided = new LinkedList;
            }
            ~Entity() = default;   
            
            void Run();
            int GetTimeDirection();
            void DeathState();
            Point GetPosition();
            vector GetSpeed();
            float GetMass();
            float GetRadius();
            void SetPosition(Point pos);
            void SetSpeed(vector vel);
            void SetMass(float m);
            void SetRadius(float r);
            void SetWidth(int w);
            void SetHeight(int h);
            void ClearCollided();
            void AddCollided(Entity*e);
            bool HasAlreadyCollided(Entity*e);
            int Top();
            int Bottom();
            int Left();
            int Right();
    };
}