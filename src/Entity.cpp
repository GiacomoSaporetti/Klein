#include "Entity.h"

using namespace Klein;

int Entity::getTimeDirection(){return directionOfTime;}

point_t Entity::getPosition()
{return position;}

vector_t Entity::getSpeed()
{return speed;}

LinkedList* Entity::getHitboxes()
{return hitboxesList;}

void Entity::Run()
{
    if(hp<=0)
    {
        goInDeathState();
        return;
    }

    float d = TIMER->getGameDelta();
    position.x += d*speed.x;
    position.y += d*speed.y;
}

/*Death State is an Entities state in which the Entity 
has reached 0 HP, but the object is not yet destroyed
in case the player uses the ability to reverse time, 
thus bringing them alive again*/
void Entity::goInDeathState()
{
    float currentGameTime = TIMER->getGameTime(); 
    if(timeOfDeath<0)
        timeOfDeath = currentGameTime;
    
    if(currentGameTime < timeOfDeath)
    {
        timeOfDeath = -1.0f;
        hp = 1;
    }
    
    if(currentGameTime > timeOfDeath + 5.0f)
        delete this;
}

void Entity::setPosition(point_t pos){position = pos;}            
            
void Entity::setSpeed(vector_t vel){speed = vel;}

void Entity::setMass(float m){mass = m;}

float Entity::getMass(){return mass;}

void Entity::clearCollided(){recentlyCollidedEntities->clear();}
            
void Entity::addCollided(Entity*e)
{
    sem_wait(&semaphore);
    recentlyCollidedEntities->appendNode(e);
    sem_post(&semaphore);
}
   
bool Entity::hasAlreadyCollided(Entity*e)
{
    sem_wait(&semaphore);

    node_t* n = recentlyCollidedEntities->getFirstNode();
    while(n != nullptr)
    {
        Entity* temp = (Entity*)n->data;
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

void Entity::addHitbox(hitbox_type_t hitboxType, point_t& center, float radius, int width, int height)
{
    Hitbox* hb = new Hitbox(this, hitboxType, center, radius, width, height);
    ASSERT(hb != nullptr)
    numberOfHitboxes++;
    hitboxesList->appendNode(hb);
}

void Entity::setFaction(int f)
{
    faction = f;
    for(int i=0; i<numberOfHitboxes; i++)
    {
        Hitbox* hb = (Hitbox*) hitboxesList->getData(i);
        hb->faction = f;
    }    
}



void Entity::updateMotion()
{
    speed = nextSpeed;
    position.x += speed.x*TIMER->getGameDelta();
    position.y += speed.y*TIMER->getGameDelta();
    nextSpeed = {0, 0};
}