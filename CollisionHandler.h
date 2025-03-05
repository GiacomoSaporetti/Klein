#pragma once
#include <stdlib.h>
#include "Klein.h"
#include "LinkedList.h"
#include "Entity.h"
#include <iostream>
#include <fstream>

namespace Klein
{
    struct EntitiesCouple
    {
        Entity* first;
        Entity* second;
    };

    class CollisionHandler
    {
        private:
            LinkedList* ENTITIES_LIST;
            int number_of_entities;    
            void AddToCheckList(LinkedList* sublist, LinkedList* CheckList);
        public:
            CollisionHandler()
            {
                ENTITIES_LIST = new LinkedList;
                number_of_entities = 0;
            }
            ~CollisionHandler() = default;   
            void SetEntitiesList(LinkedList* list) {ENTITIES_LIST=list;}
            int Run();
            int RunOptimized();
            int RunOptimized2();
            int RunOptimized3();
            int RunOptimized4();
            int CountCollision(LinkedList* list);
            bool CheckCollision(Entity* e1, Entity* e2);
    };
}