#pragma once

#include "Klein.h"
#include "TimeHandler.h"
#include "LinkedList.h"
#include "Hitbox.h"


namespace Klein
{
    const int KARMA_THRESHOLD_LOWEST = -1000;
    const int KARMA_THRESHOLD_LOW = -500;
    const int KARMA_THRESHOLD_MID = 0;
    const int KARMA_THRESHOLD_HIGH = 500;
    const int KARMA_THRESHOLD_HIGHEST = 1000;

    enum aggro_t
    {
        AGGRO_PROTECTIVE,
        AGGRO_FRIENDLY,
        AGGRO_NEUTRAL,
        AGGRO_UNFRIENDLY,
        AGGRO_AGGRESSIVE
    };

    class Entity
    {
        private:
            LinkedList* recentlyCollidedEntities;
            LinkedList* hitboxesList;
            float timeOfBirth;
            float timeOfDeath;
            int hp;
            int faction;
            int cellID;
            int numberOfHitboxes;
            sem_t semaphore;
            static int directionOfTime;
            static TimeHandler * TIMER;

            void goInDeathState();
        
        public:
            vector_t speed;
            vector_t nextSpeed;
            point_t position;
            float mass;
            float dissipationFactor;

            Entity()
            {
                sem_init(&semaphore, 0, 1);
                hp = 50;
                mass = 1.0f;
                speed.x = 0.0f;
                speed.y = 1.0f;
                position.x = 0;
                position.y = 0;

                timeOfBirth  = TIMER==NULL? -1.0f : TIMER->getGameTime();
                timeOfDeath = -1.0f;

                faction = 0;
                cellID = -1;

                recentlyCollidedEntities = new LinkedList;
                hitboxesList = new LinkedList;

                numberOfHitboxes = 0;

                nextSpeed = {0,0};
                dissipationFactor = 1;
                
            }
            ~Entity()
            {
                delete recentlyCollidedEntities;
                delete hitboxesList;
            }
            
            void Run();
            int getTimeDirection();
            point_t getPosition();
            vector_t getSpeed();
            float getMass();
            LinkedList* getHitboxes();

            void setPosition(point_t pos);
            void setSpeed(vector_t vel);
            void setMass(float m);
            
            void addHitbox(hitbox_type_t hitboxType, point_t& center, float radius, int width, int height);
            void setFaction(int f);
 
            void clearCollided();
            void addCollided(Entity*e);
            bool hasAlreadyCollided(Entity*e);  

            void updateMotion();
    };

    
}