#pragma once

#include "Klein.h"
#include "Entity.h"


namespace Klein
{

    class NPC : public Entity
    {
        public:
            AggroLevel feelingTowardsPlayer;
            
            NPC()
            {
                karma = 0;
                increaseKarma(0);
            }
            ~NPC() = default;
            void increaseKarma(int k);
            void decreaseKarma(int k);
            
            void handleIA();
        
        protected:
            int karma; //Value that determines the aggro feelingTowardsPlayer
            float tendencyToKeepAggro; //Decaying rate of aggro over time, heading towards neutral state

            void executeProtectiveIA();
            void executeFriendlyIA();
            void executeNeutralIA();
            void executeUnfriendlyIA();
            void executeAggressiveIA();

            void UpdateFeelingsTowardsPlayer();
    };
}