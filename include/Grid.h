#pragma once

#include "Klein.h"
#include "Entity.h"
#include "Hitbox.h"
#include "LinkedList.h"

namespace Klein
{
    class Cell
    {
        private:
            int id;
            LinkedList hitboxesList;
        
        public:
            Cell(int i)
            {
                id = i;
                hitboxesList = LinkedList();
            }
            ~Cell() = default;
            void addHitbox(Hitbox* h);
            void clear();
            void setId(int i);
            LinkedList* getHitboxesList();
            
    };

    class Grid
    {
        private:
            Cell* cells;
            int cellWidth;
            int numberOfCells;

            /*Rename possibly with matrix related names*/
            int numberOfVerticalCells;
            int numberOfHorizontalCells;
            
        public:
            Grid()
            {
                cellWidth = MAX_PARTICLE_SIZE;
                numberOfVerticalCells = SCREEN_Y/MAX_PARTICLE_SIZE;
                if(SCREEN_Y%MAX_PARTICLE_SIZE != 0) numberOfVerticalCells++;
                
                numberOfHorizontalCells = SCREEN_X/MAX_PARTICLE_SIZE;
                if(SCREEN_X%MAX_PARTICLE_SIZE != 0) numberOfHorizontalCells++;
                
                numberOfCells = numberOfVerticalCells*numberOfHorizontalCells;
                cells = (Cell*) std::malloc(numberOfCells*sizeof(Cell));

                for(int i=0; i<numberOfCells; i++)    cells[i] = Cell(i);
                std::cout << "Cell: " << sizeof(Cell) << std::endl;
            }
            ~Grid() = default;  

            int getNumberOfVetical(){return numberOfVerticalCells;} 
            int getNumberOfHorizontal(){return numberOfHorizontalCells;} 
            int getNumberOfCells(){return numberOfCells;} 
            void addHitboxToCell(Hitbox* h);
            inline int convertMatrixToId(int n , int m);
            LinkedList* getListOfCell(int x, int y);
            LinkedList* getListOfCell(int id);
            void clearCell(int id);
    };
}