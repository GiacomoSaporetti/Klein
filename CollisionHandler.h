#pragma once
#include <stdlib.h>
#include "Klein.h"
#include "Entity.h"

namespace Klein
{
    class CollisionHandler
    {
        private:
            Node* LIST;
            int number_of_entities;           
        public:
            CollisionHandler()
            {
                //LIST = (Entity*) malloc(MAX_NUMBER_OF_ENTITIES * sizeof(Entity*));
                LIST->data = nullptr;
                LIST->next = nullptr;
                number_of_entities = 0;
            }
            ~CollisionHandler() = default;   
    };
}