#pragma once

#include "Klein.h"
#include "Entity.h"
#include "InputHandler.h"

namespace Klein
{
    class MainCharacter: public Entity 
    {
        public:
            MainCharacter() {;}
            ~MainCharacter() = default;

            void Jump(const key_info_t& jump_key);
            void Move(const keys_state_t&  keys);
            void tick();
        
            /**
             * @brief Interagisce con l'ambiente ed oggetti circostanti
             */
            void interact();
    };
}