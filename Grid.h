#pragma once

#include "Klein.h"
#include "Entity.h"
#include "LinkedList.h"

namespace Klein
{
    class Cell
    {
        private:
            int id;
            LinkedList* entities_list;
        public:
            //static int cell_width;
            //static int number_of_cells;
            Cell(int i)
            {
                id = i;
                entities_list = new LinkedList;
            }
            ~Cell() = default;  
            void AddEntity(Entity* e);
            void Clear();
            void SetId(int i);
            LinkedList* GetList();
            
    };

    class Grid
    {
        private:
            Cell* cells_list;
            
            
        public:
            int cell_width;
            int number_of_cells;
            int vertical_cells;
            int horizontal_cells;
            Grid()
            {
                cell_width = MAX_PARTICLE_SIZE;
                vertical_cells = SCREEN_Y/MAX_PARTICLE_SIZE;
                if(SCREEN_Y%MAX_PARTICLE_SIZE != 0) vertical_cells++;
                
                horizontal_cells = SCREEN_X/MAX_PARTICLE_SIZE;
                if(SCREEN_X%MAX_PARTICLE_SIZE != 0) horizontal_cells++;
                
                number_of_cells = vertical_cells*horizontal_cells;
                cells_list = (Cell*) std::malloc(number_of_cells*sizeof(Cell));

                for(int i=0; i<number_of_cells; i++)
                    cells_list[i] = Cell(i);
                //int Cell::number_of_cells = number_of_cells;
                //int Cell::cell_width = cell_width
            }
            ~Grid() = default;   
            void AddEntityToCell(Entity* e);
            inline int ConvertMatrixToId(int n , int m);
            LinkedList* GetListOfCell(int x, int y);
            LinkedList* GetListOfCell(int id);
            void ClearCell(int id);
    };
}