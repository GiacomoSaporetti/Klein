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
    struct CollisionCouple
    {
        Hitbox* first;
        Hitbox* second;
    };

    class CollisionHandler
    {
        private:
            LinkedList* ENTITIES_LIST;
            LinkedList* HITBOXES_LIST;
            int number_of_entities;    
            
            Grid* GRID = new Grid;
            LinkedList* COLLISION_LIST;
        public:
            CollisionHandler()
            {
                ENTITIES_LIST = new LinkedList;
                number_of_entities = 0;
                COLLISION_LIST = new LinkedList;
                HITBOXES_LIST = new LinkedList;
            }
            ~CollisionHandler() = default;   
            void SetEntitiesList(LinkedList* list) {ENTITIES_LIST=list;}
            int RunNaive();
            int RunQuadrantOptimization();
            int RunGridOptimization();

            void ClearEntitiesCollisionList();
            void PrintCollisionList();

            void PopulateHitboxesList();
            void ResetMotion();
            void UpdateMotion();
    };
}