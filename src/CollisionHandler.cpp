#include "CollisionHandler.h"
#include <chrono>
#define PARTITION 2
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

//#define PRINT_COLLISION
//#define MEASURE_TIME
#define FACTIONS

#ifdef MEASURE_TIME
#define BEGIN_TIME begin = std::chrono::steady_clock::now();
#define END_TIME end = std::chrono::steady_clock::now();
#define PRINT_TIME(name)  std::cout << "Time: " << name << " "<< std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
#define GET_TIME std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
#else
#define BEGIN_TIME 
#define END_TIME 
#define PRINT_TIME(name)  
#define GET_TIME 
#endif

#ifdef PRINT_COLLISION
#define PRINT_COLLISION_LIST PrintCollisionList();
#else
#define PRINT_COLLISION_LIST
#endif

using namespace Klein;

int global_time = 0;
int global_cnt = 0;
bool CheckDistanceWithinRadius(Hitbox* h1, Hitbox* h2);
void CountCollisionThread(LinkedList* list, int* res);
bool CheckCollision(Hitbox* h1, Hitbox* h2);
int CountCollision(LinkedList* list);
void PopulateCells(Grid* grid, LinkedList*list, int start, int stop);
void CountGridMultithread(Grid* grid, int start_x, int stop_x, int start_y, int stop_y, int* res);



void ComputeCollision(Entity* e1, Entity* e2)
{
    float total_dissipation = e1->dissipation_factor*e2->dissipation_factor/2;
    vector momentum = {e1->mass*e1->speed.x + e2->mass*e2->speed.x, e1->mass*e1->speed.y + e2->mass*e2->speed.y}; 

    e1->next_speed.x += momentum.x*total_dissipation/e1->mass;
    e2->next_speed.x += momentum.x*total_dissipation/e2->mass;

    e1->next_speed.y += momentum.y*total_dissipation/e1->mass;
    e2->next_speed.y += momentum.y*total_dissipation/e2->mass;
}

void CollisionHandler::ResetMotion()
{
    Entity* e;
    Node* n = ENTITIES_LIST->GetNodeAtPosition(0);

    while(n != nullptr)
    {
        e = (Entity*) n->data;
        e->next_speed = {0, 0};
        n = n->next;
    }
}

void CollisionHandler::UpdateMotion()
{
    Entity* e;
    Node* n = ENTITIES_LIST->GetNodeAtPosition(0);

    while(n != nullptr)
    {
        e = (Entity*) n->data;
        e->UpdateMotion();
        n = n->next;
    }
}

void CollisionHandler::PopulateHitboxesList()
{
    DEBUG_MSG("PopulateHitboxesList:", "START")
    HITBOXES_LIST->Clear();
    Entity* e;
    Node* n = ENTITIES_LIST->GetNodeAtPosition(0);

    while(n != nullptr)
    {
        e = (Entity*) n->data;
        HITBOXES_LIST->AppendList(e->GetHitboxes());
        n = n->next;
    }
}


int CollisionHandler::RunNaive()
{
    DEBUG_MSG("RunNaive:", "START")
    PopulateHitboxesList();
    int res = CountCollision(HITBOXES_LIST);
    PRINT_COLLISION_LIST;
    ClearEntitiesCollisionList();
    UpdateMotion();
    return res;
}


int CollisionHandler::RunQuadrantOptimization()
{
    DEBUG_MSG("RunQuadrantOptimization:", "START")
    PopulateHitboxesList();
    int number_of_collisions = 0;
    int number_of_hitboxes = HITBOXES_LIST->GetNumberOfNodes();
    Hitbox* h;

    LinkedList* NE = new LinkedList;
    LinkedList* NO = new LinkedList;
    LinkedList* SE = new LinkedList;
    LinkedList* SO = new LinkedList;

    Point weigth_position = {0,0};

    Node* n = HITBOXES_LIST->GetNodeAtPosition(0);

    while(n != nullptr)
    {
        h = (Hitbox*) n->data;
        Point pos = h->GetPosition();
        weigth_position.x += pos.x;
        weigth_position.y += pos.y;
        n = n->next;
    }
    weigth_position.x = weigth_position.x/number_of_hitboxes;
    weigth_position.y = weigth_position.y/number_of_hitboxes;
  
    n = HITBOXES_LIST->GetNodeAtPosition(0);
    
    while(n != nullptr)
    {
        h = (Hitbox*) n->data;
        float radius = h->GetRadius();
        Point center = h->GetPosition();
        bool N=false;
        bool E=false;
        bool S=false;
        bool O=false;
        
        if(center.x + radius >= weigth_position.x)
            E = true;    
        if(center.x - radius < weigth_position.x)    
            O = true;
        if(center.y + radius >= weigth_position.y)
            N = true;    
        if(center.y - radius < weigth_position.y)    
            S = true;

        if(N && O)  NO->AddNodeEnd(h);
        if(N && E)  NE->AddNodeEnd(h);
        if(S && O)  SO->AddNodeEnd(h);
        if(S && E)  SE->AddNodeEnd(h);
        n = n->next;
    }
    

    int nne=0, nno=0, nse=0, nso=0;

    /*if(MAX_NUMBER_THREADS >= 4)
    {
        THREADS[0] = std::thread(CountCollisionThread, NE, &nne);
        THREADS[1] = std::thread(CountCollisionThread, NO, &nno);
        THREADS[2] = std::thread(CountCollisionThread, SE, &nse);
        THREADS[3] = std::thread(CountCollisionThread, SO, &nso);

        THREADS[0].join();
        THREADS[1].join();
        THREADS[2].join();
        THREADS[3].join();
    }
    if(MAX_NUMBER_THREADS == 2)
    {*/
        THREADS[0] = std::thread(CountCollisionThread, NE, &nne);
        THREADS[1] = std::thread(CountCollisionThread, NO, &nno);
        THREADS[0].join();
        THREADS[1].join();
        THREADS[0] = std::thread(CountCollisionThread, SE, &nse);
        THREADS[1] = std::thread(CountCollisionThread, SO, &nso);
        THREADS[0].join();
        THREADS[1].join();
    /*}
    if(MAX_NUMBER_THREADS == 1)
    {*/
        /*nne = CountCollision(NE);
        nno = CountCollision(NO);
        nse = CountCollision(SE);
        nso = CountCollision(SO);*/
    //}

    number_of_collisions = nne + nno + nse + nso;

    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    UpdateMotion();
    return number_of_collisions;
}

void PopulateCells(Grid* grid, LinkedList*list, int start, int stop)
{
    Hitbox* h;
    Node* current_node = list->GetNodeAtPosition(start);

    for(int i=start; i<stop; i++)
    {
        h = (Hitbox*)current_node->data;
        if(h != nullptr)
            grid->AddHitboxToCell(h);
        current_node = current_node->next;
        if(current_node == nullptr)
            break;
    }
}


void CountGridMultithread(Grid* grid, int start_x, int stop_x, int start_y, int stop_y, int* res)
{
    //auto my_begin = std::chrono::steady_clock::now();
    int number_of_collisions = 0;
    for(int x=start_x; x<=stop_x; x+=2)
    {
        for(int y=start_y; y<=stop_y; y+=2)
        {
            LinkedList* CheckList = new LinkedList;
            CheckList->AppendList(grid->GetListOfCell(x-1, y-1));
            CheckList->AppendList(grid->GetListOfCell(x-1, y));
            CheckList->AppendList(grid->GetListOfCell(x-1, y+1));
            CheckList->AppendList(grid->GetListOfCell(x, y-1));
            CheckList->AppendList(grid->GetListOfCell(x, y));
            CheckList->AppendList(grid->GetListOfCell(x, y+1));
            CheckList->AppendList(grid->GetListOfCell(x+1, y-1));
            CheckList->AppendList(grid->GetListOfCell(x+1, y));
            CheckList->AppendList(grid->GetListOfCell(x+1, y+1));
            
            number_of_collisions += CountCollision(CheckList);
           
            delete CheckList;
        }
    }

    *res = number_of_collisions;
    //auto my_end = std::chrono::steady_clock::now();
    //std::cout << "Time:  "<< std::chrono::duration_cast<std::chrono::microseconds>(my_end - my_begin).count() << std::endl;
}


int CollisionHandler::RunGridOptimization()
{
    DEBUG_MSG("RunGridOptimization:", "START")
    PopulateHitboxesList();

    int number_of_collisions = 0;

    Hitbox* h;
    int active_cnt = 0;
    Node*n = HITBOXES_LIST->GetNodeAtPosition(0);

    while(n != nullptr)
    {
        h = (Hitbox*) n->data;
        if(h != nullptr)
            GRID->AddHitboxToCell(h);
        n = n->next;
    }

    int temp_res=0;
    for(int x=1; x<=GRID->horizontal_cells; x+=2)
    {
        for(int y=1; y<=GRID->vertical_cells; y+=2)
        {
            LinkedList* CheckList = new LinkedList;
            CheckList->AppendList(GRID->GetListOfCell(x-1, y-1));
            CheckList->AppendList(GRID->GetListOfCell(x-1, y));
            CheckList->AppendList(GRID->GetListOfCell(x-1, y+1));
            CheckList->AppendList(GRID->GetListOfCell(x, y-1));
            CheckList->AppendList(GRID->GetListOfCell(x, y));
            CheckList->AppendList(GRID->GetListOfCell(x, y+1));
            CheckList->AppendList(GRID->GetListOfCell(x+1, y-1));
            CheckList->AppendList(GRID->GetListOfCell(x+1, y));
            CheckList->AppendList(GRID->GetListOfCell(x+1, y+1));
            
            int number_of_nodes = CheckList->GetNumberOfNodes();
            int number_of_checks = number_of_nodes*number_of_nodes/2;

            number_of_collisions += CountCollision(CheckList);
            delete CheckList;
        }
    }

    for(int i=0; i<GRID->number_of_cells; i++)
        GRID->ClearCell(i);
    
    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    UpdateMotion();
    return number_of_collisions;  
}


bool CheckCollision(Hitbox* h1, Hitbox* h2)
{
    DEBUG_MSG("CheckCollision:", "START")
    
    Entity*e1 = (Entity*) h1->GetParentEntity();
    Entity*e2 = (Entity*) h2->GetParentEntity();

    if(e1 == e2)
        return false;
 
    if(e2->HasAlreadyCollided(e1))
        return false;
    
    int type1 = h1->type;
    int type2 = h2->type;
    
    /*Both are circles*/
    if(type1 == CIRCLE && type2 == CIRCLE)
        return CheckDistanceWithinRadius(h1, h2);

    /*Check the boundaries, if no match they do NOT collide
    either if they are 2 rectangles or 1 circle and 1 rectangle*/
    if(h1->GetTop() < h2->GetBottom())
        return false;
    if(h2->GetTop() < h1->GetBottom())
        return false;
    if(h1->GetRight() < h2->GetLeft())
        return false;
    if(h2->GetRight() < h1->GetLeft())
        return false;   
    
    /*If they are different shapes che the distance between center*/
    if(type1 != type2)
        return CheckDistanceWithinRadius(h1, h2);
    
    return true;
}

bool CheckDistanceWithinRadius(Hitbox* h1, Hitbox* h2)
{
    DEBUG_MSG("CheckDistanceWithinRadius:", "START")
  
    Point p1 = h1->GetPosition();
    Point p2 = h2->GetPosition();
    vector distance={float(p1.x - p2.x), float(p1.y - p2.y)};

    if(distance.Magnitude() < (h1->GetRadius() + h2->GetRadius()))
            return true;

    return false;
}

void CountCollisionThread(LinkedList* list, int* res)
{
    DEBUG_MSG("CountCollision(single):", "START")
    int number_of_collisions = 0;
    Entity* active_entity;
    Entity* check_entity;
    Hitbox* active_hitbox;
    Hitbox* check_hitbox;
    int active_cnt = 0;
    int check_cnt = 0;
    
    Node* active_node = list->GetNodeAtPosition(0);
    Node* check_node;

    while(active_node !=  nullptr)
    {
        active_hitbox = (Hitbox*) active_node->data;
        active_entity = (Entity*) active_hitbox->GetParentEntity();
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
                        COLLISION_LIST->AddNodeEnd(couple);
                    #endif
                    check_entity = (Entity*) check_hitbox->GetParentEntity();
                    active_entity->AddCollided(check_entity);
                    check_entity->AddCollided(active_entity);
                    ComputeCollision(active_entity, check_entity);
                    number_of_collisions++;
                }
        }
        active_node = active_node->next;
    }
    
    *res =  number_of_collisions;
}


int CountCollision(LinkedList* list)
{
    DEBUG_MSG("CountCollision(single):", "START")
    int number_of_collisions = 0;
    Entity* active_entity;
    Entity* check_entity;
    Hitbox* active_hitbox;
    Hitbox* check_hitbox;
    int active_cnt = 0;
    int check_cnt = 0;
    
    Node* active_node = list->GetNodeAtPosition(0);
    Node* check_node;

    while(active_node !=  nullptr)
    {
        active_hitbox = (Hitbox*) active_node->data;
        active_entity = (Entity*) active_hitbox->GetParentEntity();
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
                        COLLISION_LIST->AddNodeEnd(couple);
                    #endif
                    check_entity = (Entity*) check_hitbox->GetParentEntity();
                    active_entity->AddCollided(check_entity);
                    check_entity->AddCollided(active_entity);
                    ComputeCollision(active_entity, check_entity);
                    number_of_collisions++;
                }  
        }

        active_node = active_node->next;
    }
    
    return number_of_collisions;
}

void CollisionHandler::ClearEntitiesCollisionList()
{
    DEBUG_MSG("ClearEntitiesCollisionList:", "START")
    Node* n = ENTITIES_LIST->GetNodeAtPosition(0);
    Entity* ent;
    int cnt=0;
    while(n != nullptr)
    {
        ent = (Entity*) n->data;
        ent->ClearCollided();
        n = n->next;
    }
}

void CollisionHandler::PrintCollisionList()
{
    DEBUG_MSG("PrintCollisionList:", "START")
    CollisionCouple*couple;
    int cnt=0;
    while((couple=(CollisionCouple*)COLLISION_LIST->GetData(cnt++)) != nullptr)
    {
        Hitbox* h1 = couple->first;
        Hitbox* h2 = couple->second;
        std::cout << h1 << " | ";
        std::cout << "c= " << h1->center.x << ", " << h1->center.y << " | ";
        std::cout << "t= " << h1->type << " r= " << h1->radius << " ";
        std::cout << "h= " << h1->height << " w= " << h1->width << " | ";
        std::cout << "cell= " << h1->cell_id << " /";

        std::cout << h2 << " | ";
        std::cout << "c= " << h2->center.x << ", " << h2->center.y << " | ";
        std::cout << "t= " << h2->type << " r= " << h2->radius << " ";
        std::cout << "h= " << h2->height << " w= " << h2->width << " | ";
        std::cout << "cell= " << h2->cell_id << " /";
        std::cout <<std::endl;
    }

    COLLISION_LIST->Clear();
}

