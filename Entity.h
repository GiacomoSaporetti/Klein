#pragma once

#include "Klein.h"
#include "TimeHandler.h"
#include "LinkedList.h"
#include "Hitbox.h"

namespace Klein
{
    class Entity
    {
        private:
            LinkedList* recently_collided;
            LinkedList* hitboxes;
        public:
            vector speed;
            Point position;
            float birth;
            float death;
            float mass;
            int hp=50;
            int faction;
            int cell_id;
            int number_of_hitboxes;
            static int time_direction;
            static TimeHandler * TIMER;
            Entity()
            {
                mass = 1.0f;
                speed.x = 0.0f;
                speed.y = 1.0f;
                position.x = 0;
                position.y = 0;

                if(TIMER != NULL)
                    birth  = TIMER->GetGameTime();
                else birth = -1.0f;

                death = -1.0f;
                faction = 0;
                cell_id = -1;

                recently_collided = new LinkedList;
                hitboxes = new LinkedList;

                number_of_hitboxes = 0;
            }
            ~Entity() = default;   
            
            void Run();
            int GetTimeDirection();
            void DeathState();
            Point GetPosition();
            vector GetSpeed();
            float GetMass();
            LinkedList* GetHitboxes();
            //float GetRadius();
            void SetPosition(Point pos);
            void SetSpeed(vector vel);
            void SetMass(float m);
            
            void AddHitbox(HITBOX_TYPE TYPE, Point* CENTER, float RADIUS, int WIDTH, int HEIGHT);
            void SetFaction(int f);
            /*void SetRadius(float r);
            void SetWidth(int w);
            void SetHeight(int h);*/
            void ClearCollided();
            void AddCollided(Entity*e);
            bool HasAlreadyCollided(Entity*e);
            /*int Top();
            int Bottom();
            int Left();
            int Right();*/
    };
}