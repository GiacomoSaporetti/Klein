#include "Entity.h"
#include "Hitbox.h"

namespace Klein
{
    /*Membri statici*/

    int          Entity::s_timeDirection = 1;
    TimeHandler* Entity::s_timer         = nullptr; //Questo va assegnato dopo aver istanziato il timer globale!!!
    std::mutex   Entity::s_mutex;
    /*Costruttore*/
    Entity::Entity()  : m_timeOfBirth(s_timer != nullptr ? s_timer->getGameTime() : -1.f) {}
    
    Entity::~Entity() = default;

    /*Getters*/

    int          Entity::getTimeDirection()         { return s_timeDirection; }
    point_t      Entity::getPosition()      const   { return m_position; }
    vector_t     Entity::getSpeed()         const   { return m_speed; }
    float        Entity::getMass()          const   { return m_mass; }
    float        Entity::getDissipation()   const   { return m_dissipation; }
    int          Entity::getHp()            const   { return m_hp; }
    int          Entity::getFaction()       const   { return m_faction; }
    int          Entity::getCellID()        const   { return m_cellID; }
    float        Entity::getTimeOfBirth()   const   { return m_timeOfBirth; }

    const std::vector<Hitbox*>& Entity::getHitboxes() const { return m_hitboxes; }

    /*Setters*/

    void Entity::setPosition(point_t pos)       { m_position      = pos;   }
    void Entity::setSpeed(vector_t vel)         { m_speed         = vel;   }
    void Entity::setAcceleration(vector_t acc)  { m_acceleration  = acc;   }
    void Entity::setMass(float m)               { m_mass          = m;     }
    void Entity::setCellID(int id)              { m_cellID        = id;    }

    void Entity::setFaction(int faction)
    {
        m_faction = faction;
        for (Hitbox* hb : m_hitboxes)
            hb->setFaction(faction);
    }

    void Entity::setTimer(TimeHandler* timer)
    {
        s_mutex.lock();
        s_timer = timer;
        s_mutex.unlock();
    }

    void Entity::setTimeDirection(int dir)
    {
        s_mutex.lock();
        s_timeDirection = dir;
        s_mutex.unlock();
    }


    void Entity::addHitbox(Hitbox* hb) { KLEIN_ASSERT(hb != nullptr); m_hitboxes.push_back(hb); }


    void Entity::clearCollidedList()
    {
        m_currentFrameCollisionList.clear();
    }

    void Entity::addCollided(Entity* other)
    {
        s_mutex.lock();
        m_currentFrameCollisionList.push_back(other);
        s_mutex.unlock();
    }

    bool Entity::hasAlreadyCollided(const Entity* other) const
    {
        s_mutex.lock();
        for (const Entity* e : m_currentFrameCollisionList)
        {
            if (e == other)
            {
                s_mutex.unlock();
                return true;
            }
        }
        s_mutex.unlock();
        return false;
    }


    void Entity::updateMotion()
    {
        const long dt = s_timer->getGameDelta();
        m_speed.x     += m_acceleration.x * dt * 1e-9;
        m_speed.y     += m_acceleration.y * dt* 1e-9;
        m_position.x  += m_speed.x * dt * 1e-9;
        m_position.y  += m_speed.y * dt * 1e-9;
        m_acceleration = {0.f, 0.f};
    }


    void Entity::tick()
    {
        if (m_hp <= 0)
        {
            goInDeathState();
            return;
        }
        updateMotion();
    }


    void Entity::goInDeathState()
    {
        const float now = s_timer->getGameTime();

        if (m_timeOfDeath < 0.f)
            m_timeOfDeath = now;

        // Resurrezione tramite inversione temporale
        if (now < m_timeOfDeath)
        {
            m_timeOfDeath = -1.f;
            m_hp          = 1;
            return;
        }

        // Dopo 5 secondi l'istanza viene distrutta
        if (now > m_timeOfDeath + 5.f)
            delete this;
    }

} //namespace Klein