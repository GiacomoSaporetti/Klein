#pragma once
#include <stdlib.h>
#include "Klein.h"
#include "LinkedList.h"
#include "Entity.h"
#include "Grid.h"
#include <iostream>
#include <fstream>


/*For performance measurement*/
extern std::chrono::steady_clock::time_point timeMeasureBegin;
extern std::chrono::steady_clock::time_point timeMeasureEnd;

#define MEASURE_TIME
#ifdef MEASURE_TIME
#define BEGIN_TIME timeMeasureBegin = std::chrono::steady_clock::now();
#define END_TIME timeMeasureEnd = std::chrono::steady_clock::now();
#define GET_TIME std::chrono::duration_cast<std::chrono::microseconds>(timeMeasureEnd - timeMeasureBegin).count()
#define PRINT_TIME(name)  std::cout << "Time: " << name << " "<< GET_TIME << std::endl;
#else
#define BEGIN_TIME 
#define END_TIME 
#define PRINT_TIME(name)  
#define GET_TIME 
#endif

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
            LinkedList HITBOXES_LIST;
            LinkedList COLLISION_LIST;
            
            Grid* GRID;
            
            int numberOfEntities;    
            
            void printCollisionList();
        
        public:
            CollisionHandler()
            {
                numberOfEntities =  0;
                COLLISION_LIST   =  LinkedList();
                HITBOXES_LIST    =  LinkedList();

                GRID = new Grid;
                ASSERT(GRID != nullptr)
            }
            ~CollisionHandler() = default;
            
            void setEntitiesList(LinkedList& list) {ENTITIES_LIST=&list;}
            int runNaiveImplementation();
            int runQuadrantOptimization();
            int runGridOptimization();

            void clearEntitiesCollisionList();

            int populateHitboxesList();
            void resetMotion();
            void updateMotion();
    };
}