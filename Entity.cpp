#include "Entity.h"

using namespace Klein;

int Entity::GetTimeDirection()
{
    return time_direction;
}

Point Entity::GetPosition()
{return position;}

vector Entity::GetSpeed()
{return speed;}

LinkedList* Entity::GetHitboxes()
{return hitboxes;}

void Entity::Run()
{
    if(hp<=0)
    {
        DeathState();
        return;
    }

    float d = TIMER->GetGameDelta();
    position.x += d*speed.x;
    position.y += d*speed.y;
}

void Entity::DeathState()
{
    float time = TIMER->GetGameTime(); 
    if(death<0)
        death = time;
    
    if(time < death)
    {
        death = -1.0f;
        hp = 1;
    }
    
    if(time > death + 5.0f)
        delete this;
}

void Entity::SetPosition(Point pos){position = pos;}            
            
void Entity::SetSpeed(vector vel){speed = vel;}

void Entity::SetMass(float m){mass = m;}

/*void Entity::SetRadius(float r){hitbox.SetRadius(r);}
void Entity::SetWidth(int w){hitbox.SetWidth(w);}
void Entity::SetHeight(int h){hitbox.SetHeight(h);}*/

float Entity::GetMass(){return mass;}
//float Entity::GetRadius(){return hitbox.radius;}

void Entity::ClearCollided()
{
    DEBUG_MSG("ClearCollided: ", "");
    recently_collided->Clear();
}
            
void Entity::AddCollided(Entity*e)
{
    DEBUG_MSG("AddCollided: ", e);
    recently_collided->AddNodeEnd(e);
}
        
bool Entity::HasAlreadyCollided(Entity*e)
{
    DEBUG_MSG("HasAlreadyCollided: ", e);
    Entity* temp;
    Node* n = recently_collided->GetNodeAtPosition(0);
    while(n != nullptr)
    {
        temp = (Entity*)n->data;
        if(temp == e)
            return true;
        n = n->next;
    }
    return false;
}

void Entity::AddHitbox(HITBOX_TYPE TYPE, Point* CENTER, float RADIUS, int WIDTH, int HEIGHT)
{
    number_of_hitboxes++;
    Hitbox* hb = new Hitbox(this, TYPE, CENTER, RADIUS, WIDTH, HEIGHT);
    hitboxes->AddNodeEnd(hb);
}

void Entity::SetFaction(int f)
{
    faction = f;
    for(int i=0; i<number_of_hitboxes; i++)
    {
        Hitbox* hb = (Hitbox*) hitboxes->GetData(i);
        hb->faction = f;
    }    
}

/*int Entity::Top()
{return hitbox.Top();}
int Entity::Bottom()
{return hitbox.Bottom();}
int Entity::Left()
{return hitbox.Left();}
int Entity::Right()
{return hitbox.Right();}*/