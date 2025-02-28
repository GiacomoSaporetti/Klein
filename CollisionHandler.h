#pragma once
#include <stdlib.h>
#include "Klein.h"
#include "LinkedList.h"
#include "Entity.h"

namespace Klein
{
    class CollisionHandler
    {
        private:
            LinkedList* ENTITIES_LIST;
            int number_of_entities;          
        public:
            CollisionHandler()
            {
                ENTITIES_LIST = new LinkedList;
                number_of_entities = 0;
            }
            ~CollisionHandler() = default;   
    };
}