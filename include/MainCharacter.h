#pragma once

#include "Klein.h"
#include "Entity.h"

namespace Klein
{
    class MainCharacter: public Entity 
    {
        public:
            MainCharacter() {;}
            ~MainCharacter() = default;

            void Move(int up, int down, int left, int right);
    };
}