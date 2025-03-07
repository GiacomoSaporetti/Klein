#include "Grid.h"
using namespace Klein;

void Grid::AddEntityToCell(Entity* e)
{
    Point position = e->GetPosition();
    int hor = int(position.x)/cell_width;
    int ver = int(position.y)/cell_width;
    int id = ConvertMatrixToId(hor, ver);
    if(id < 0 || id > (number_of_cells - 1)) return;
    /*std::cout << "Added entity: " << e << " to cell " << id;
    std::cout << " (" << hor << ", " << ver << ")";
    std::cout << " -- " << position.x << ", "<< position.y <<std::endl;*/
    cells_list[id].AddEntity(e);
}

inline int Grid::ConvertMatrixToId(int x , int y)
{
    if(x<0 || x>=horizontal_cells)
        return -1;
    if(y<0 || y>=vertical_cells)
        return -1;
    return  x + y*horizontal_cells;
}

void Cell::AddEntity(Entity* e)
{entities_list->AddNodeEnd(e);
e->cell_id = id;}

void Cell::SetId(int i)
{id = i;}

void Cell::Clear()
{
    entities_list->Clear();
}

LinkedList* Cell::GetList()
{return entities_list;}

LinkedList* Grid::GetListOfCell(int x, int y)
{
    int id = ConvertMatrixToId(x, y);
    if(id < 0 || id > (number_of_cells - 1))
        return nullptr;
    return cells_list[id].GetList();
}

LinkedList* Grid::GetListOfCell(int id)
{
    if(id < 0 || id > (number_of_cells - 1))
        return nullptr;
    return cells_list[id].GetList();
}

void Grid::ClearCell(int id)
{
    if(id < 0 || id > (number_of_cells - 1))
        return;
    cells_list[id].Clear();
}