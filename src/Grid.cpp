#include "Grid.h"
using namespace Klein;

void Grid::addHitboxToCell(Hitbox* h)
{
    point_t position = h->getPosition();
    int hor = int(position.x)/cellWidth;
    int ver = int(position.y)/cellWidth;
    int id = convertMatrixToId(hor, ver);
    if(id < 0 || id > (numberOfCells - 1))    return;
    /*std::cout << "Added entity: " << e << " to cell " << id;
    std::cout << " (" << hor << ", " << ver << ")";
    std::cout << " -- " << position.x << ", "<< position.y <<std::endl;*/
    cells[id].addHitbox(h);
}

inline int Grid::convertMatrixToId(int x , int y)
{
    if(x<0 || x>=numberOfHorizontalCells)  return -1;
    if(y<0 || y>=numberOfVerticalCells)    return -1;
    return  x + y*numberOfHorizontalCells;
}

void Cell::addHitbox(Hitbox* h){hitboxesList.appendNode(h); h->cellID=id;}

void Cell::setId(int i){id = i;}

void Cell::clear(){hitboxesList.clear();}

LinkedList* Cell::getHitboxesList(){return &hitboxesList;}

LinkedList* Grid::getListOfCell(int x, int y)
{
    int id = convertMatrixToId(x, y);
    if(id < 0 || id > (numberOfCells - 1))    return nullptr;
    return cells[id].getHitboxesList();
}

LinkedList* Grid::getListOfCell(int id)
{
    if(id < 0 || id > (numberOfCells - 1))    return nullptr;
    return cells[id].getHitboxesList();
}

void Grid::clearCell(int id)
{
    if(id < 0 || id > (numberOfCells - 1))    return;
    cells[id].clear();
}