#include "Entity.h"

using namespace Klein;

int Entity::GetTimeDirection(){return time_direction;}

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

float Entity::GetMass(){return mass;}

void Entity::ClearCollided()
{
    DEBUG_MSG("ClearCollided: ", "");
    recently_collided->Clear();
}
            
void Entity::AddCollided(Entity*e)
{
    DEBUG_MSG("AddCollided: ", e);

    sem_wait(&semaphore);
    recently_collided->AddNodeEnd(e);
    sem_post(&semaphore);
}
   
bool Entity::HasAlreadyCollided(Entity*e)
{
    sem_wait(&semaphore);
    DEBUG_MSG("HasAlreadyCollided: ", e);
    Entity* temp;
    Node* n = recently_collided->GetNodeAtPosition(0);
    while(n != nullptr)
    {
        temp = (Entity*)n->data;
        if(temp == e)
            {
                sem_post(&semaphore);
                return true;
            }
        n = n->next;
    }
    sem_post(&semaphore);
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



void Entity::UpdateMotion()
{
    speed = next_speed;
    position.x += speed.x*TIMER->GetGameDelta();
    position.y += speed.y*TIMER->GetGameDelta();
    next_speed = {0, 0};
}