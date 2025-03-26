#pragma once

#include "Klein.h"
#include "Entity.h"


namespace Klein
{
    #define KARMA_THRESHOLD_4 -1000
    #define KARMA_THRESHOLD_3 -500
    #define KARMA_THRESHOLD_2 0
    #define KARMA_THRESHOLD_1 500
    #define KARMA_THRESHOLD_0 1000

    enum Aggro
    {
        PROTECTIVE,
        FRIENDLY,
        NEUTRAL,
        UNFRIENDLY,
        AGGRESSIVE
    };

    class NPC : public Entity
    {
        public:
            enum Aggro feeling;
            int Karma;


            NPC()
            {
                Karma = 0;
                IncreaseKarma(0);
            }
            ~NPC() = default;
            void IncreaseKarma(int k);
            void DecreaseKarma(int k);
            
            void HandleIA();
        
        protected:
            void ProtectiveIA();
            void FriendlyIA();
            void NeutralIA();
            void UnfriendlyIA();
            void AggressiveIA();
    };
}