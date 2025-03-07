#pragma once
#include <stdlib.h>
#include "Klein.h"
#include "LinkedList.h"
#include "Entity.h"
#include "Grid.h"
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
            Grid* GRID = new Grid;
            LinkedList* COLLISION_LIST;
        public:
            CollisionHandler()
            {
                ENTITIES_LIST = new LinkedList;
                number_of_entities = 0;
                COLLISION_LIST = new LinkedList;
            }
            ~CollisionHandler() = default;   
            void SetEntitiesList(LinkedList* list) {ENTITIES_LIST=list;}
            int Run();
            int RunOptimized();
            int RunOptimized2();
            int RunOptimized3();
            int RunOptimized4();
            int RunOptimized5();
            int RunOptimized6(int depth);
            int CountCollision(LinkedList* list);
            int CountCollision(LinkedList* active,LinkedList* passive);
            bool CheckCollision(Entity* e1, Entity* e2);
            void ClearEntitiesCollisionList();
            int SubProcessForOptimization6(int depth, LinkedList*list);
            void PrintCollisionList();
    };
}