#include "Entity.h"

using namespace Klein;

int Entity::GetTimeDirection()
{
    return time_direction;
}

vector Entity::GetPosition()
{return position;}

vector Entity::GetSpeed()
{return speed;}

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