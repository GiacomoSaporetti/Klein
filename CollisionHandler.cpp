#include "CollisionHandler.h"
#define PARTITION 2

using namespace Klein;

int CollisionHandler::Run()
{
    
    
    int number_of_collisions = 0;
    //naive
    Entity* active_entity;
    Entity* check_entity;
    int active_cnt = 0;
    int check_cnt = 0;

    while((active_entity = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        float active_radius = active_entity->GetRadius();
        vector active_center = active_entity->GetPosition();
        check_cnt = active_cnt+1;
        while((check_entity = (Entity*)ENTITIES_LIST->GetData(check_cnt)) != nullptr)
        {
            
            float total_radius = active_radius + check_entity->GetRadius();
            vector check_center = check_entity->GetPosition();
            vector distance = {active_center.x - check_center.x,active_center.y - check_center.y};
            if(distance.Magnitude()<= total_radius)
            {
                number_of_collisions++;
                
            }
            check_cnt++;
        }
        active_cnt++;
    }

    //File.close();
    return number_of_collisions;
    
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
        vector center = e->GetPosition();

        if(center.x + radius > 50)
            List1->AddNodeEnd(e);
        if(center.x - radius < 50)    
            List2->AddNodeEnd(e);
        if((center.x + radius > 50) && (center.x - radius < 50))
            List3->AddNodeEnd(e);
    }

    number_of_collisions += CountCollision(List1);
    number_of_collisions += CountCollision(List2);
    number_of_collisions -= CountCollision(List3);
    return number_of_collisions;
}

int CollisionHandler::RunOptimized2()
{
    //Split in 2 [0, 50] and [50, 100]
    //std::ofstream File("Coll.txt");
    
    int number_of_collisions = 0;
    Entity* e;
    int active_cnt = 0;
    LinkedList* NE = new LinkedList;
    LinkedList* NO = new LinkedList;
    LinkedList* SE = new LinkedList;
    LinkedList* SO = new LinkedList;
    LinkedList* VERTICAL = new LinkedList;
    LinkedList* HORIZONTAL = new LinkedList;
    LinkedList* CENTER = new LinkedList;
    vector weigth_position = {0,0};
    while((e = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        vector pos = e->GetPosition();
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
        vector center = e->GetPosition();
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
        if(N && S)  HORIZONTAL->AddNodeEnd(e);
        if(E && O)  VERTICAL->AddNodeEnd(e);
        if(N && S && O && E) CENTER->AddNodeEnd(e);
    }

    
    number_of_collisions += CountCollision(NE);
    
    number_of_collisions += CountCollision(NO);
    
    number_of_collisions += CountCollision(SE);
    
    number_of_collisions += CountCollision(SO);
    
    number_of_collisions -= CountCollision(HORIZONTAL);
    
    number_of_collisions -= CountCollision(VERTICAL);
    
    number_of_collisions -= CountCollision(CENTER);
    
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
    LinkedList* V1 = new LinkedList;
    LinkedList* V2 = new LinkedList;
    LinkedList* H = new LinkedList;
    LinkedList* C1 = new LinkedList;
    LinkedList* C2 = new LinkedList;
    vector c1 = {0,0};
    vector c2 = {0,0};
    vector weight_position = {0,0};

    while((ent = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        vector pos = ent->GetPosition();
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
        vector center = ent->GetPosition();

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
        if(h)  H->AddNodeEnd(ent);
        if(v1) V1->AddNodeEnd(ent);
        if(v2) V2->AddNodeEnd(ent);
        if(h && v1) C1->AddNodeEnd(ent);
        if(h && v2) C2->AddNodeEnd(ent);

    }

    number_of_collisions += CountCollision(A);
    number_of_collisions += CountCollision(B);
    number_of_collisions += CountCollision(C);
    number_of_collisions += CountCollision(D);
    number_of_collisions += CountCollision(E);
    number_of_collisions += CountCollision(F);
    number_of_collisions -= CountCollision(V1);
    number_of_collisions -= CountCollision(V2);
    number_of_collisions -= CountCollision(H);
    number_of_collisions -= CountCollision(C1);
    number_of_collisions -= CountCollision(C2);
    

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
    LinkedList* V1 = new LinkedList;
    LinkedList* V2 = new LinkedList;
    LinkedList* H1 = new LinkedList;
    LinkedList* H2 = new LinkedList;
    LinkedList* C1 = new LinkedList;
    LinkedList* C2 = new LinkedList;
    LinkedList* C3 = new LinkedList;
    LinkedList* C4 = new LinkedList;
    vector c1 = {0,0};
    vector c2 = {0,0};
    vector c3 = {0,0};
    vector c4 = {0,0};
    vector weight_position = {0,0};

    while((ent = (Entity*)ENTITIES_LIST->GetData(active_cnt)) != nullptr)
    {
        vector pos = ent->GetPosition();
        weight_position.x += pos.x;
        weight_position.y += pos.y;
        active_cnt++;
    }
    weight_position.x = weight_position.x/active_cnt;
    weight_position.y = weight_position.y/active_cnt; 

    float h1 = weight_position.y/2;
    float h2 = (100+weight_position.y)/2;
    float v1 = weight_position.x/2;
    float v2 = (100+weight_position.x)/2;

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
        float radius = ent->GetRadius();
        vector center = ent->GetPosition();

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
        
        bool h1_flag = (d && g)||(e && h)||(i && f);
        bool h2_flag = (d && a)||(e && b)||(c && f);
        bool v1_flag = (a && b)||(d && e)||(g && h);
        bool v2_flag = (c && b)||(f && e)||(i && h);

        if(a)  A->AddNodeEnd(ent);
        if(b)  B->AddNodeEnd(ent);
        if(c)  C->AddNodeEnd(ent);
        if(d)  D->AddNodeEnd(ent);
        if(e)  E->AddNodeEnd(ent);
        if(f)  F->AddNodeEnd(ent);
        if(g)  G->AddNodeEnd(ent);
        if(h)  H->AddNodeEnd(ent);
        if(i)  I->AddNodeEnd(ent);
        if(v1_flag) V1->AddNodeEnd(ent);
        if(v2_flag) V2->AddNodeEnd(ent);
        if(h1_flag) H1->AddNodeEnd(ent);
        if(h2_flag) H2->AddNodeEnd(ent);
        if(h1_flag && v1_flag) C3->AddNodeEnd(ent);
        if(h1_flag && v2_flag) C4->AddNodeEnd(ent);
        if(h2_flag && v1_flag) C1->AddNodeEnd(ent);
        if(h2_flag && v2_flag) C2->AddNodeEnd(ent);

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
    number_of_collisions -= CountCollision(V1);
    number_of_collisions -= CountCollision(V2);
    number_of_collisions -= CountCollision(H1);
    number_of_collisions -= CountCollision(H2);
    number_of_collisions -= CountCollision(C1);
    number_of_collisions -= CountCollision(C2);
    number_of_collisions -= CountCollision(C3);
    number_of_collisions -= CountCollision(C4);
    

    return number_of_collisions;
}

void CollisionHandler::AddToCheckList(LinkedList* sublist, LinkedList* CheckList)
{
    int number_of_nodes = sublist->GetNumberOfNodes();

    Entity* active_e;
    Entity* passive_e;
    for(int i=0; i<number_of_nodes; i++)
    {
        active_e = (Entity*)sublist->GetData(i);
        for(int j=i+1; j<number_of_nodes; j++)
        {
            passive_e = (Entity*)sublist->GetData(j);

        /*    bool found = false;
            EntitiesCouple* read;
            int cnt=0;
            while((read=(EntitiesCouple*)CheckList->GetData(cnt++)) != nullptr)
            {
                if(read->first == active_e)
                {    
                    if(read->second == passive_e)
                    {
                        found = true;
                        break;
                    }
                }
            }*/
            
            EntitiesCouple* couple = new EntitiesCouple;
            couple->first = active_e;
            couple->second = passive_e;
            CheckList->AddNodeEnd(couple);
        }
    }
}

bool CollisionHandler::CheckCollision(Entity* e1, Entity* e2)
{
    
    float total_radius = e1->GetRadius() + e2->GetRadius();
    vector distance;
    vector p1 = e1->GetPosition();
    vector p2 = e2->GetPosition();
    distance.x = p1.x - p2.x;
    distance.y = p1.y - p2.y;
    
    if(distance.Magnitude() < total_radius)
        {
            
            return true;
        }
    
    return false; 
}

int CollisionHandler::CountCollision(LinkedList* list)
{
    
    int number_of_collisions = 0;
    //naive
    Entity* active_entity;
    Entity* check_entity;
    int active_cnt = 0;
    int check_cnt = 0;

    while((active_entity = (Entity*)list->GetData(active_cnt)) != nullptr)
    {
        check_cnt = active_cnt+1;
        while((check_entity = (Entity*)list->GetData(check_cnt++)) != nullptr)
        {
            if(CheckCollision(active_entity, check_entity))
                number_of_collisions++;
        }
        active_cnt++;
    }

    return number_of_collisions;
}