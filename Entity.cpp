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

void Entity::SetPosition(Point pos){position = pos; hitbox.center = pos;}            
            
void Entity::SetSpeed(vector vel){speed = vel;}

void Entity::SetMass(float m){mass = m;}

void Entity::SetRadius(float r){hitbox.SetRadius(r);}
void Entity::SetWidth(int w){hitbox.SetWidth(w);}
void Entity::SetHeight(int h){hitbox.SetHeight(h);}

float Entity::GetMass(){return mass;}
float Entity::GetRadius(){return hitbox.radius;}

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
    int cnt=0;
    Entity*temp;
    while((temp = (Entity*)recently_collided->GetData(cnt++))!= nullptr)
    {
        if(temp == e)
            return true;
    }
    return false;
}

int Entity::Top()
{return hitbox.Top();}
int Entity::Bottom()
{return hitbox.Bottom();}
int Entity::Left()
{return hitbox.Left();}
int Entity::Right()
{return hitbox.Right();}