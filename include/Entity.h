#pragma once

#include "Klein.h"
#include "TimeHandler.h"
#include "LinkedList.h"
#include "Hitbox.h"

namespace Klein
{
    class Entity
    {
        protected:
            LinkedList* recently_collided;
            LinkedList* hitboxes;
            sem_t semaphore;
        public:
            vector speed;
            vector next_speed;
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
            float dissipation_factor;
            Entity()
            {
                sem_init(&semaphore, 0, 1);
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

                next_speed = {0,0};
                dissipation_factor = 1;
                
            }
            ~Entity() = default;   

            void Run();
            int GetTimeDirection();
            void DeathState();
            Point GetPosition();
            vector GetSpeed();
            float GetMass();
            LinkedList* GetHitboxes();

            void SetPosition(Point pos);
            void SetSpeed(vector vel);
            void SetMass(float m);
            
            void AddHitbox(HITBOX_TYPE TYPE, Point* CENTER, float RADIUS, int WIDTH, int HEIGHT);
            void SetFaction(int f);
 
            void ClearCollided();
            void AddCollided(Entity*e);
            bool HasAlreadyCollided(Entity*e);  

            void UpdateMotion();
    };

    
}