#include "CollisionHandler.h"
#include <chrono>
#define PARTITION 2
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

//#define PRINT_COLLISION
//#define FACTIONS
#ifdef PRINT_COLLISION
#define PRINT_COLLISION_LIST PrintCollisionList();
#else
#define PRINT_COLLISION_LIST
#endif
using namespace Klein;

bool CheckDistance(Entity* e1, Entity* e2);

int CollisionHandler::Run()
{
    int res = CountCollision(ENTITIES_LIST);
    PrintCollisionList();
    ClearEntitiesCollisionList();
    return res;
}

int CollisionHandler::RunOptimized()
{
    //Split in 2 [0, 50] and [50, 100]
    int number_of_collisions = 0;
    Entity* e;
    int active_cnt = 0;
    LinkedList* List1 = new LinkedList;
    LinkedList* List2 = new LinkedList;
    LinkedList* List3 = new LinkedList;

    while((e = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        active_cnt++;
        float radius = e->GetRadius();
        Point center = e->GetPosition();

        if(center.x + radius > 50)
            List1->AddNodeEnd(e);
        if(center.x - radius < 50)    
            List2->AddNodeEnd(e);
    }

    number_of_collisions += CountCollision(List1);
    number_of_collisions += CountCollision(List2);

    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    return number_of_collisions;
}

int CollisionHandler::RunOptimized2()
{
    int number_of_collisions = 0;
    Entity* e;
    int active_cnt = 0;
    LinkedList* NE = new LinkedList;
    LinkedList* NO = new LinkedList;
    LinkedList* SE = new LinkedList;
    LinkedList* SO = new LinkedList;

    Point weigth_position = {0,0};
    while((e = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        Point pos = e->GetPosition();
        weigth_position.x += pos.x;
        weigth_position.y += pos.y;
        active_cnt++;
    }
    weigth_position.x = weigth_position.x/active_cnt;
    weigth_position.y = weigth_position.y/active_cnt;
    //weigth_position.x = 50;
    //weigth_position.y = 50;
    
    active_cnt = 0;
    while((e = (Entity*)ENTITIES_LIST->GetData(active_cnt++)) != nullptr)
    {
        float radius = e->GetRadius();
        Point center = e->GetPosition();
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

        if(N && O)  NO->AddNodeEnd(e);
        if(N && E)  NE->AddNodeEnd(e);
        if(S && O)  SO->AddNodeEnd(e);
        if(S && E)  SE->AddNodeEnd(e);
    }

    
    number_of_collisions += CountCollision(NE);
    number_of_collisions += CountCollision(NO);
    number_of_collisions += CountCollision(SE);
    number_of_collisions += CountCollision(SO);
    

    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    return number_of_collisions;
}

int CollisionHandler::RunOptimized3()
{
    int number_of_collisions = 0;
    Entity* ent;
    int active_cnt = 0;
    LinkedList* A = new LinkedList;
    LinkedList* B = new LinkedList;
    LinkedList* C = new LinkedList;
    LinkedList* D = new LinkedList;
    LinkedList* E = new LinkedList;
    LinkedList* F = new LinkedList;

    Point c1 = {0,0};
    Point c2 = {0,0};
    Point weight_position = {0,0};

    while((ent = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        Point pos = ent->GetPosition();
        weight_position.x += pos.x;
        weight_position.y += pos.y;
        active_cnt++;
    }
    weight_position.x = weight_position.x/active_cnt;
    weight_position.y = weight_position.y/active_cnt; 

    c1.y = weight_position.y;
    c2.y = weight_position.y;
    c1.x = weight_position.x/2;
    c2.x = (100+weight_position.x)/2;
   
    /*c1.y = 50;
    c2.y = 50;
    c1.x = 33;
    c2.x = 66;*/
    //weigth_position.x = 50;
    //weigth_position.y = 50;
    
    active_cnt = 0;
    while((ent = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        active_cnt++;
        float radius = ent->GetRadius();
        Point center = ent->GetPosition();

        bool v1=false, v2=false, h=false;
        bool a=true, b=true, c=true, d=true, e=true, f=true;

        
        if(center.x + radius < c2.x){c=false; f=false;}
        if(center.x + radius < c1.x){b=false; e=false;}
        if(center.x - radius > c1.x){a=false; d=false;}
        if(center.x - radius > c2.x){b=false; e=false;}

        if(center.y + radius < c1.y){a=false; b=false; c=false;}
        if(center.y - radius > c1.y){d=false; e=false; f=false;}
        
        h=(a && d)||(b && e)||(c && f);
        v1 = (a && b)||(d && e);
        v2 = (c && b)||(f && e);

        if(a)  A->AddNodeEnd(ent);
        if(b)  B->AddNodeEnd(ent);
        if(c)  C->AddNodeEnd(ent);
        if(d)  D->AddNodeEnd(ent);
        if(e)  E->AddNodeEnd(ent);
        if(f)  F->AddNodeEnd(ent);
    }

    number_of_collisions += CountCollision(A);
    number_of_collisions += CountCollision(B);
    number_of_collisions += CountCollision(C);
    number_of_collisions += CountCollision(D);
    number_of_collisions += CountCollision(E);
    number_of_collisions += CountCollision(F);
    
    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    return number_of_collisions;
}

int CollisionHandler::RunOptimized4()
{
    int number_of_collisions = 0;
    Entity* ent;
    int active_cnt = 0;
    LinkedList* A = new LinkedList;
    LinkedList* B = new LinkedList;
    LinkedList* C = new LinkedList;
    LinkedList* D = new LinkedList;
    LinkedList* E = new LinkedList;
    LinkedList* F = new LinkedList;
    LinkedList* G = new LinkedList;
    LinkedList* H = new LinkedList;
    LinkedList* I = new LinkedList;
    /*LinkedList* V1 = new LinkedList;
    LinkedList* V2 = new LinkedList;
    LinkedList* H1 = new LinkedList;
    LinkedList* H2 = new LinkedList;
    LinkedList* C1 = new LinkedList;
    LinkedList* C2 = new LinkedList;
    LinkedList* C3 = new LinkedList;
    LinkedList* C4 = new LinkedList;*/
    Point c1 = {0,0};
    Point c2 = {0,0};
    Point c3 = {0,0};
    Point c4 = {0,0};
    Point weight_position = {0,0};

    while((ent = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        Point pos = ent->GetPosition();
        weight_position.x += pos.x;
        weight_position.y += pos.y;
        active_cnt++;
    }
    weight_position.x = weight_position.x/active_cnt;
    weight_position.y = weight_position.y/active_cnt; 

    int h1 = weight_position.y/2;
    int h2 = (100+weight_position.y)/2;
    int v1 = weight_position.x/2;
    int v2 = (100+weight_position.x)/2;

    c1.y = h2;
    c1.x = v1;

    c2.y = h2;
    c2.x = v2;
    
    c3.y = h1;
    c3.x = v1;

    c4.y = h1;
    c4.x = v2;
   
    active_cnt = 0;
    while((ent = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        active_cnt++;
        int radius = ent->GetRadius();
        Point center = ent->GetPosition();

        //bool v1=false, v2=false, h1=false, h2=false;
        bool a=true, b=true, c=true, d=true, e=true, f=true, g=true, h=true, i=true;
        
        if(center.x + radius < v2){c=false; f=false; i=false;}
        if(center.x + radius < v1){b=false; e=false; h=false;}
        if(center.x - radius > v1){a=false; d=false; g=false;}
        if(center.x - radius > v2){b=false; e=false; h=false;}

        if(center.y + radius < h2){a=false; b=false; c=false;}
        if(center.y + radius < h1){d=false; e=false; f=false;}
        if(center.y - radius > h1){g=false; h=false; i=false;}
        if(center.y - radius > h2){d=false; e=false; f=false;}
        
        /*bool h1_flag = (d && g)||(e && h)||(i && f);
        bool h2_flag = (d && a)||(e && b)||(c && f);
        bool v1_flag = (a && b)||(d && e)||(g && h);
        bool v2_flag = (c && b)||(f && e)||(i && h);*/

        if(a)  A->AddNodeEnd(ent);
        if(b)  B->AddNodeEnd(ent);
        if(c)  C->AddNodeEnd(ent);
        if(d)  D->AddNodeEnd(ent);
        if(e)  E->AddNodeEnd(ent);
        if(f)  F->AddNodeEnd(ent);
        if(g)  G->AddNodeEnd(ent);
        if(h)  H->AddNodeEnd(ent);
        if(i)  I->AddNodeEnd(ent);
        /*if(v1_flag) V1->AddNodeEnd(ent);
        if(v2_flag) V2->AddNodeEnd(ent);
        if(h1_flag) H1->AddNodeEnd(ent);
        if(h2_flag) H2->AddNodeEnd(ent);
        if(h1_flag && v1_flag) C3->AddNodeEnd(ent);
        if(h1_flag && v2_flag) C4->AddNodeEnd(ent);
        if(h2_flag && v1_flag) C1->AddNodeEnd(ent);
        if(h2_flag && v2_flag) C2->AddNodeEnd(ent);*/

    }

    number_of_collisions += CountCollision(A);
    number_of_collisions += CountCollision(B);
    number_of_collisions += CountCollision(C);
    number_of_collisions += CountCollision(D);
    number_of_collisions += CountCollision(E);
    number_of_collisions += CountCollision(F);
    number_of_collisions += CountCollision(G);
    number_of_collisions += CountCollision(H);
    number_of_collisions += CountCollision(I);
    /*number_of_collisions -= CountCollision(V1);
    number_of_collisions -= CountCollision(V2);
    number_of_collisions -= CountCollision(H1);
    number_of_collisions -= CountCollision(H2);
    number_of_collisions -= CountCollision(C1);
    number_of_collisions -= CountCollision(C2);
    number_of_collisions -= CountCollision(C3);
    number_of_collisions -= CountCollision(C4);*/
    
    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    return number_of_collisions;
}

int CollisionHandler::RunOptimized5()
{
    int number_of_collisions = 0;
    Entity* ent;
    int active_cnt = 0;
 
    while((ent = (Entity*)ENTITIES_LIST->GetData(active_cnt++)) != nullptr)
    {
        GRID->AddEntityToCell(ent);
    }

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
            
            //begin = std::chrono::steady_clock::now();
            number_of_collisions += CountCollision(CheckList);
            //end = std::chrono::steady_clock::now();

            //std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
            delete CheckList;
        }
    }

    for(int i=0; i<GRID->number_of_cells; i++)
        GRID->ClearCell(i);
    
    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    return number_of_collisions;  
}

int CollisionHandler::RunOptimized6(int depth)
{
    int number_of_collisions = SubProcessForOptimization6(depth-1, ENTITIES_LIST);
    PRINT_COLLISION_LIST
    ClearEntitiesCollisionList();
    return number_of_collisions;
}

int CollisionHandler::SubProcessForOptimization6(int depth, LinkedList*list)
{
    if(depth<=0)
        return CountCollision(list);
    int number_of_collisions = 0;
    Entity* e;
    int active_cnt = 0;
    LinkedList* NE = new LinkedList;
    LinkedList* NO = new LinkedList;
    LinkedList* SE = new LinkedList;
    LinkedList* SO = new LinkedList;
    
    Point weigth_position = {0,0};
    while((e = (Entity*)list->GetData(active_cnt)) != nullptr)
    {
        Point pos = e->GetPosition();
        weigth_position.x += pos.x;
        weigth_position.y += pos.y;
        active_cnt++;
    }
    weigth_position.x = weigth_position.x/active_cnt;
    weigth_position.y = weigth_position.y/active_cnt;
    
    active_cnt = 0;
    while((e = (Entity*)ENTITIES_LIST->GetData(active_cnt++)) != nullptr)
    {
        float radius = e->GetRadius();
        Point center = e->GetPosition();
        bool N=false;
        bool E=false;
        bool S=false;
        bool O=false;
        
        if(center.x + radius >= weigth_position.x) E = true;    
        if(center.x - radius < weigth_position.x)  O = true;
        if(center.y + radius >= weigth_position.y) N = true;    
        if(center.y - radius < weigth_position.y)  S = true;

        if(N && O)  NO->AddNodeEnd(e);
        if(N && E)  NE->AddNodeEnd(e);
        if(S && O)  SO->AddNodeEnd(e);
        if(S && E)  SE->AddNodeEnd(e);
    }

    number_of_collisions += SubProcessForOptimization6(depth-1, NE);
    number_of_collisions += SubProcessForOptimization6(depth-1, NO);
    number_of_collisions += SubProcessForOptimization6(depth-1, SE); 
    number_of_collisions += SubProcessForOptimization6(depth-1, SO); 

    return number_of_collisions;
}

void CollisionHandler::AddToCheckList(LinkedList* sublist, LinkedList* CheckList)
{
}

bool CollisionHandler::CheckCollision(Entity* e1, Entity* e2)
{
     if(e1->HasAlreadyCollided(e2))
        return false;

    int type1 = e1->hitbox.type;
    int type2 = e2->hitbox.type;
    
    /*Both are circles*/
    if(type1 == CIRCLE && type2 == CIRCLE)
        return CheckDistance(e1, e2);

    /*Check the boundaries, if no match they do NOT collide
    either if they are 2 rectangles or 1 circle and 1 rectangle*/
    if(e1->Top()<e2->Bottom())
        return false;
    if(e2->Top()<e1->Bottom())
        return false;
    if(e1->Right()<e2->Left())
        return false;
    if(e2->Right()<e1->Left())
        return false;
        
    /*If they are different shapes che the distance between center*/
    if(type1 != type2)
        return CheckDistance(e1, e2);
    
    return true;
    
}

bool CheckDistance(Entity* e1, Entity* e2)
{
    //std::cout << "CheckDistance: " << e1 << " " << e2 << std::endl;

    Point p1 = e1->GetPosition();
    Point p2 = e2->GetPosition();
    vector distance={float(p1.x - p2.x), float(p1.y - p2.y)};

    //std::cout << "CheckDistance: distance " << distance.x << " " << distance.y << " " << distance.Magnitude()<< std::endl;

    if(distance.Magnitude() < (e1->GetRadius() + e2->GetRadius()))
            {
                //std::cout << "CheckDistance: total radius" << e1->GetRadius() + e2->GetRadius() << std::endl;
                return true;
            }

    return false;
}

int CollisionHandler::CountCollision(LinkedList* list)
{
    int number_of_collisions = 0;
    Entity* active_entity;
    Entity* check_entity;
    int active_cnt = 0;
    int check_cnt = 0;
    
    //t1 = std::chrono::steady_clock::now();
    while((active_entity = (Entity*)list->GetData(active_cnt++)) != nullptr)
    {
        #ifdef FACTIONS
            int active_faction = active_entity->faction;
        #endif
        check_cnt = active_cnt;
        while((check_entity = (Entity*)list->GetData(check_cnt++)) != nullptr)
        {
            #ifdef FACTIONS
                if(active_faction == check_entity->faction)
                    continue;
            #endif
            
            if(CheckCollision(active_entity, check_entity))
                {
                    #ifdef PRINT_COLLISION
                        EntitiesCouple* couple = new EntitiesCouple;
                        couple->first = active_entity;
                        couple->second = check_entity;
                        COLLISION_LIST->AddNodeEnd(couple);
                    #endif
                    
                    active_entity->AddCollided(check_entity);
                    check_entity->AddCollided(active_entity);
                    number_of_collisions++;
                }
        }
    }
    
    //t2 = std::chrono::steady_clock::now(); 
    //    std::cout << "check"<<std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << std::endl;
   
    return number_of_collisions;
}


int CollisionHandler::CountCollision(LinkedList* active, LinkedList* passive)
{
    int number_of_collisions = 0;
    Entity* active_entity;
    Entity* check_entity;
    int active_cnt = 0;
    int check_cnt = 0;

    while((active_entity = (Entity*)active->GetData(active_cnt++)) != nullptr)
    {
        int active_faction = active_entity->faction;
        check_cnt = 0;
        while((check_entity = (Entity*)passive->GetData(check_cnt++)) != nullptr)
        {
            #ifdef FACTIONS
                if(active_faction == check_entity->faction)
                    continue;
            #endif
            #ifdef PRINT_COLLISION
                std::cout << active_entity << " " << check_entity<< " ";
                std::cout << active_entity->position.x << " " << active_entity->position.y<< " ";
                std::cout << active_entity->GetRadius()<< " ";
                std::cout << check_entity->position.x << " " << check_entity->position.y<< " ";
                std::cout << check_entity->GetRadius()<< " ";
                std::cout << "| " <<  active_entity->cell_id << " " << check_entity->cell_id<< " ";
                std::cout <<std::endl;
            #endif
            if(CheckCollision(active_entity, check_entity))
                {
                    #ifdef PRINT_COLLISION
                        std::cout << "OK" << std::endl;
                    #endif
                    number_of_collisions++;
                }
        }
    }

    return number_of_collisions;
}

void CollisionHandler::ClearEntitiesCollisionList()
{
    Entity*ent;
    int cnt=0;
    while((ent = (Entity*)ENTITIES_LIST->GetData(cnt++)) != nullptr)
    {
        ent->ClearCollided();
    }
}

void CollisionHandler::PrintCollisionList()
{
    EntitiesCouple*couple;
    int cnt=0;
    while((couple=(EntitiesCouple*)COLLISION_LIST->GetData(cnt++)) != nullptr)
    {
        Entity*active_entity = couple->first;
        Entity*check_entity = couple->second;
        std::cout << active_entity << " " << check_entity<< " ";
        std::cout << "| " << active_entity->position.x << " " << active_entity->position.y<< " ";
        std::cout << active_entity->GetRadius()<< " ";
        std::cout << active_entity->hitbox.type<< " ";
        std::cout << "| " << check_entity->position.x << " " << check_entity->position.y<< " ";
        std::cout << check_entity->GetRadius()<< " ";
        std::cout << check_entity->hitbox.type<< " ";
        std::cout << "| " <<  active_entity->cell_id << " " << check_entity->cell_id<< " ";
        std::cout <<std::endl;
    }

    COLLISION_LIST->Clear();
}

