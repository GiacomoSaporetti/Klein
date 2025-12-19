#include "CollisionHandler.h"
#include <chrono>

//#define PRINT_COLLISION
//#define MEASURE_TIME
#define FACTIONS

std::chrono::steady_clock::time_point timeMeasureBegin;
std::chrono::steady_clock::time_point timeMeasureEnd;


#ifdef PRINT_COLLISION
#define PRINT_COLLISION_LIST printCollisionList();
#else
#define PRINT_COLLISION_LIST
#endif

using namespace Klein;

int global_time = 0;
int global_cnt = 0;

/*Prototypes*/
bool CheckDistanceWithinRadius(Hitbox* h1, Hitbox* h2);
void CountCollisionThread(LinkedList* list, int* res);
bool CheckCollision(Hitbox* h1, Hitbox* h2);
int  CountCollision(LinkedList& list);
void PopulateCells(Grid* grid, LinkedList* list, int start, int stop);
void CountGridMultithread(Grid* grid, int start_x, int stop_x, int start_y, int stop_y, int* res);
void ComputeCollision(Entity* e1, Entity* e2);


void ComputeCollision(Entity* e1, Entity* e2)
{
    float total_dissipation = e1->dissipationFactor*e2->dissipationFactor/2;
    vector_t momentum = {e1->mass*e1->speed.x + e2->mass*e2->speed.x, e1->mass*e1->speed.y + e2->mass*e2->speed.y}; 

    e1->nextSpeed.x += momentum.x*total_dissipation/e1->mass;
    e2->nextSpeed.x += momentum.x*total_dissipation/e2->mass;

    e1->nextSpeed.y += momentum.y*total_dissipation/e1->mass;
    e2->nextSpeed.y += momentum.y*total_dissipation/e2->mass;
}

void CollisionHandler::resetMotion()
{
    node_t* currentNode = ENTITIES_LIST->getFirstNode();

    while(currentNode != nullptr)
    {
        Entity* e = (Entity*) currentNode->data;
        ASSERT(e != nullptr)
        e->nextSpeed = {0, 0};
        currentNode = currentNode->next;
    }
}

void CollisionHandler::updateMotion()
{
    node_t* currentNode = ENTITIES_LIST->getFirstNode();

    while(currentNode != nullptr)
    {
        Entity* e = (Entity*) currentNode->data;
        ASSERT(e!=nullptr)
        e->updateMotion();
        currentNode = currentNode->next;
    }
}

int CollisionHandler::populateHitboxesList()
{
    HITBOXES_LIST.clear();
    ASSERT(HITBOXES_LIST.getFirstNode() == nullptr)

    node_t* currentNode = ENTITIES_LIST->getFirstNode();

    while(currentNode != nullptr)
    {
        Entity* e = (Entity*) currentNode->data;
        HITBOXES_LIST.appendList(*e->getHitboxes());
        currentNode = currentNode->next;
    }

    return HITBOXES_LIST.getNumberOfNodes();
}


int CollisionHandler::runNaiveImplementation()
{
    if(populateHitboxesList() == 0) return 0;
    int res = CountCollision(HITBOXES_LIST);
    
    PRINT_COLLISION_LIST;
    clearEntitiesCollisionList();
    updateMotion();
    return res;
}


int CollisionHandler::runQuadrantOptimization()
{
    if(populateHitboxesList() == 0) return 0;

    int numberOfCollisions = 0;
    int numberOfHitboxes = HITBOXES_LIST.getNumberOfNodes();
    Hitbox* h;

    LinkedList* NorthEast = new LinkedList;
    LinkedList* NorthWest = new LinkedList;
    LinkedList* SouthEast = new LinkedList;
    LinkedList* SouthWest = new LinkedList;

    point_t meanWeightedPosition = {0,0};

    node_t* currentNode = HITBOXES_LIST.getFirstNode();

    while(currentNode != nullptr)
    {
        h = (Hitbox*) currentNode->data;
        point_t pos = h->getPosition();
        meanWeightedPosition.x += pos.x;
        meanWeightedPosition.y += pos.y;
        currentNode = currentNode->next;
    }
    meanWeightedPosition.x = meanWeightedPosition.x/numberOfHitboxes;
    meanWeightedPosition.y = meanWeightedPosition.y/numberOfHitboxes;
  
    currentNode = HITBOXES_LIST.getFirstNode();
    
    while(currentNode != nullptr)
    {
        h = (Hitbox*) currentNode->data;
        float radius = h->getRadius();
        point_t center = h->getPosition();
        bool N=false;
        bool E=false;
        bool S=false;
        bool O=false;
        
        if(center.x + radius >= meanWeightedPosition.x)      E = true;    
        if(center.x - radius <  meanWeightedPosition.x)      O = true;
        if(center.y + radius >= meanWeightedPosition.y)      N = true;    
        if(center.y - radius <  meanWeightedPosition.y)      S = true;

        if(N && O)  NorthWest->appendNode(h);
        if(N && E)  NorthEast->appendNode(h);
        if(S && O)  SouthWest->appendNode(h);
        if(S && E)  SouthEast->appendNode(h);
        currentNode = currentNode->next;
    }
    

    int nne=0, nno=0, nse=0, nso=0;

    /*if(MAX_NUMBER_THREADS >= 4)
    {
        THREADS[0] = std::thread(CountCollisionThread, NorthEast, &nne);
        THREADS[1] = std::thread(CountCollisionThread, NorthWest, &nno);
        THREADS[2] = std::thread(CountCollisionThread, SouthEast, &nse);
        THREADS[3] = std::thread(CountCollisionThread, SouthWest, &nso);

        THREADS[0].join();
        THREADS[1].join();
        THREADS[2].join();
        THREADS[3].join();
    }
    if(MAX_NUMBER_THREADS == 2)
    {*/
        THREADS[0] = std::thread(CountCollisionThread, NorthEast, &nne);
        THREADS[1] = std::thread(CountCollisionThread, NorthWest, &nno);
        THREADS[0].join();
        THREADS[1].join();
        THREADS[0] = std::thread(CountCollisionThread, SouthEast, &nse);
        THREADS[1] = std::thread(CountCollisionThread, SouthWest, &nso);
        THREADS[0].join();
        THREADS[1].join();
    /*}
    if(MAX_NUMBER_THREADS == 1)
    {*/
        /*nne = CountCollision(NorthEast);
        nno = CountCollision(NorthWest);
        nse = CountCollision(SouthEast);
        nso = CountCollision(SouthWest);*/
    //}

    numberOfCollisions = nne + nno + nse + nso;

    PRINT_COLLISION_LIST
    clearEntitiesCollisionList();
    updateMotion();


    delete NorthEast;
    delete NorthWest;
    delete SouthEast;
    delete SouthWest;
    return numberOfCollisions;
}

void PopulateCells(Grid* grid, LinkedList*list, int start, int stop)
{
    Hitbox* h;
    node_t* current_node = list->getNodeAtPosition(start);

    for(int i=start; i<stop; i++)
    {
        h = (Hitbox*)current_node->data;
        if(h != nullptr)
            grid->addHitboxToCell(h);
        current_node = current_node->next;
        if(current_node == nullptr)
            break;
    }
}


void CountGridMultithread(Grid* grid, int start_x, int stop_x, int start_y, int stop_y, int* res)
{
    //auto my_begin = std::chrono::steady_clock::now();
    int numberOfCollisions = 0;
    for(int x=start_x; x<=stop_x; x+=2)
    {
        for(int y=start_y; y<=stop_y; y+=2)
        {
            LinkedList CheckList = LinkedList();
            CheckList.appendList(grid->getListOfCell(x-1, y-1));
            CheckList.appendList(grid->getListOfCell(x-1, y));
            CheckList.appendList(grid->getListOfCell(x-1, y+1));
            CheckList.appendList(grid->getListOfCell(x, y-1));
            CheckList.appendList(grid->getListOfCell(x, y));
            CheckList.appendList(grid->getListOfCell(x, y+1));
            CheckList.appendList(grid->getListOfCell(x+1, y-1));
            CheckList.appendList(grid->getListOfCell(x+1, y));
            CheckList.appendList(grid->getListOfCell(x+1, y+1));
            
            numberOfCollisions += CountCollision(CheckList);
        }
    }

    *res = numberOfCollisions;
    //auto my_end = std::chrono::steady_clock::now();
    //std::cout << "Time:  "<< std::chrono::duration_cast<std::chrono::microseconds>(my_end - my_begin).count() << std::endl;
}


int CollisionHandler::runGridOptimization()
{
    if(populateHitboxesList() == 0) return 0;
    
    int numberOfCollisions = 0;

    node_t* currentNode = HITBOXES_LIST.getFirstNode();

    while(currentNode != nullptr)
    {
        Hitbox* h = (Hitbox*) currentNode->data;
        if(h != nullptr)    GRID->addHitboxToCell(h);
        currentNode = currentNode->next;
    }
    

    for(int x=1; x<=GRID->getNumberOfHorizontal(); x+=2)
    {
        for(int y=1; y<=GRID->getNumberOfVetical(); y+=2)
        {
            LinkedList CheckList = LinkedList();
            CheckList.appendList(GRID->getListOfCell(x-1, y-1));
            CheckList.appendList(GRID->getListOfCell(x-1, y));
            CheckList.appendList(GRID->getListOfCell(x-1, y+1));
            CheckList.appendList(GRID->getListOfCell(x, y-1));
            CheckList.appendList(GRID->getListOfCell(x, y));
            CheckList.appendList(GRID->getListOfCell(x, y+1));
            CheckList.appendList(GRID->getListOfCell(x+1, y-1));
            CheckList.appendList(GRID->getListOfCell(x+1, y));
            CheckList.appendList(GRID->getListOfCell(x+1, y+1));
            
            numberOfCollisions += CountCollision(CheckList);
        }
    }

    for(int i=0; i<GRID->getNumberOfCells(); i++)  GRID->clearCell(i);
    
    PRINT_COLLISION_LIST
    clearEntitiesCollisionList();
    updateMotion();
    return numberOfCollisions;  
}


bool CheckCollision(Hitbox* h1, Hitbox* h2)
{
    Entity*e1 = (Entity*) h1->getParentEntity();
    Entity*e2 = (Entity*) h2->getParentEntity();

    if(e1 == e2)
        return false;
 
    if(e2->hasAlreadyCollided(e1))
        return false;
    
    int type1 = h1->type;
    int type2 = h2->type;
    
    /*Both are circles*/
    if(type1 == CIRCLE && type2 == CIRCLE)
        return CheckDistanceWithinRadius(h1, h2);

    /*Check the boundaries, if no match they do NOT collide
    either if they are 2 rectangles or 1 circle and 1 rectangle*/
    if(h1->getTop() < h2->getBottom())  return false;
    if(h2->getTop() < h1->getBottom())  return false;
    if(h1->getRight() < h2->getLeft())  return false;
    if(h2->getRight() < h1->getLeft())  return false;   
    
    /*If they are different shapes check the distance between center*/
    if(type1 != type2)  return CheckDistanceWithinRadius(h1, h2);
    
    return true;
}

bool CheckDistanceWithinRadius(Hitbox* h1, Hitbox* h2)
{
    point_t p1 = h1->getPosition();
    point_t p2 = h2->getPosition();
    vector_t distance={float(p1.x - p2.x), float(p1.y - p2.y)};

    if(distance.getMagnitude() < (h1->getRadius() + h2->getRadius()))   return true;

    return false;
}

void CountCollisionThread(LinkedList* list, int* res)
{
    int numberOfCollisions = 0;
    Entity* active_entity;
    Entity* check_entity;
    Hitbox* active_hitbox;
    Hitbox* check_hitbox;
    int active_cnt = 0;
    int check_cnt = 0;
    
    node_t* active_node = list->getFirstNode();
    node_t* check_node;

    while(active_node !=  nullptr)
    {
        active_hitbox = (Hitbox*) active_node->data;
        active_entity = (Entity*) active_hitbox->getParentEntity();
        int active_faction = active_hitbox->faction;
        check_node = active_node;
        while((check_node = check_node->next)!= nullptr)
        {
            check_hitbox = (Hitbox*) check_node->data;
            int check_faction = check_hitbox->faction;
            if(active_faction == check_faction)
                    continue;       
            if(CheckCollision(active_hitbox, check_hitbox))
            {
                #ifdef PRINT_COLLISION
                CollisionCouple* couple = new CollisionCouple;
                couple->first = active_hitbox;
                couple->second = check_hitbox;
                COLLISION_LIST.appendNode(couple);
                #endif
                
                check_entity = (Entity*) check_hitbox->getParentEntity();
                active_entity->addCollided(check_entity);
                check_entity->addCollided(active_entity);
                ComputeCollision(active_entity, check_entity);
                numberOfCollisions++;
            }
        }
        active_node = active_node->next;
    }
    
    *res =  numberOfCollisions;
}


int CountCollision(LinkedList& list)
{
    int numberOfCollisions = 0;
    Entity* active_entity;
    Entity* check_entity;
    Hitbox* active_hitbox;
    Hitbox* check_hitbox;
    int active_cnt = 0;
    int check_cnt = 0;
    
    node_t* active_node = list.getFirstNode();
    node_t* first_node = active_node;
    node_t* check_node;


    while(active_node !=  nullptr)
    {
        if(first_node == active_node) break; /*Prevents infinite loops*/
        
        active_hitbox = (Hitbox*) active_node->data;
        active_entity = (Entity*) active_hitbox->getParentEntity();
        int active_faction = active_hitbox->faction;
        check_node = active_node;
        printf("active %p\n", active_node);
        while((check_node = check_node->next)!= nullptr)
        {
            if(check_node == active_node) break; /*Prevents infinite loops*/
            
            check_hitbox = (Hitbox*) check_node->data;
            int check_faction = check_hitbox->faction;
            if(active_faction == check_faction)
                    continue;    
           
            if(CheckCollision(active_hitbox, check_hitbox))
            {
                #ifdef PRINT_COLLISION
                CollisionCouple* couple = new CollisionCouple;
                couple->first = active_hitbox;
                couple->second = check_hitbox;
                COLLISION_LIST.appendNode(couple);
                #endif

                check_entity = (Entity*) check_hitbox->getParentEntity();
                active_entity->addCollided(check_entity);
                check_entity->addCollided(active_entity);
                ComputeCollision(active_entity, check_entity);
                numberOfCollisions++;
            }  
        }

        active_node = active_node->next;
    }
    
    return numberOfCollisions;
}

void CollisionHandler::clearEntitiesCollisionList()
{
    node_t* currentNode = ENTITIES_LIST->getFirstNode();
    Entity* ent;
    int cnt=0;
    while(currentNode != nullptr)
    {
        ent = (Entity*) currentNode->data;
        ent->clearCollided();
        currentNode = currentNode->next;
    }
}

/*For debug use only*/
void CollisionHandler::printCollisionList()
{
    CollisionCouple*couple;
    int cnt=0;
    while((couple=(CollisionCouple*)COLLISION_LIST.getData(cnt++)) != nullptr)
    {
        Hitbox* h1 = couple->first;
        Hitbox* h2 = couple->second;
        std::cout << h1 << " | ";
        std::cout << "c= " << h1->center.x << ", " << h1->center.y << " | ";
        std::cout << "t= " << h1->type << " r= " << h1->radius << " ";
        std::cout << "h= " << h1->height << " w= " << h1->width << " | ";
        std::cout << "cell= " << h1->cellID << " /";

        std::cout << h2 << " | ";
        std::cout << "c= " << h2->center.x << ", " << h2->center.y << " | ";
        std::cout << "t= " << h2->type << " r= " << h2->radius << " ";
        std::cout << "h= " << h2->height << " w= " << h2->width << " | ";
        std::cout << "cell= " << h2->cellID << " /";
        std::cout <<std::endl;
    }

    COLLISION_LIST.clear();
}
