#include "Entity.h"
#include "Hitbox.h"

namespace Klein
{
    extern TimeHandler      g_timer;
    std::vector<Entity*>    g_all_entities;
    std::vector<Hitbox*>    g_walls; 

    void AddEntity(Entity* e)    {g_all_entities.push_back(e);}

    std::vector<Entity*>& GetAllEntities() {return g_all_entities;}
    
    Entity* CreateWall(rectangle_t wall)
    {
        Entity* e = new Entity();
        int   n_vertical_hb   = std::floor(wall.height()/MAX_PARTICLE_SIZE) + 1;
        int   n_horizontal_hb = std::floor(wall.width()/MAX_PARTICLE_SIZE) + 1;
        float hb_height       = wall.height()/n_vertical_hb;
        float hb_width        = wall.width()/n_horizontal_hb;

        for(int h=0; h<n_horizontal_hb; h++)
        {
            for(int v=0; v<n_vertical_hb; v++)
            {
                rectangle_t hb_area;
                hb_area.setOrigin({h*hb_width, v*hb_height});
                hb_area.setHeight(hb_height);
                hb_area.setWidth(hb_width);

                Hitbox* hb = new Hitbox(*e, hb_area);
            }
        }
        return e;
    }


    /*Membri statici*/

    int          Entity::s_timeDirection = 1;
    std::mutex   Entity::s_mutex;
    /*Costruttore*/
    Entity::Entity()  : m_timeOfBirth(g_timer.getGameTime()), m_is_klein(false){}
    
    Entity::~Entity()
    {
        for(auto h : this->m_hitboxes)
            delete h;
    }

    /*Getters*/

    int          Entity::getTimeDirection()         { return s_timeDirection; }
    point_t      Entity::getPosition()      const   { return m_position; }
    float        Entity::getMass()          const   { return m_mass; }
    float        Entity::getDissipation()   const   { return m_dissipation; }
    int          Entity::getHp()            const   { return m_hp; }
    int          Entity::getFaction()       const   { return m_faction; }
    float        Entity::getTimeOfBirth()   const   { return m_timeOfBirth; }
    vector_t     Entity::getSpeed()         const   
    { 
        /*if(!m_is_klein)  return m_speed*g_timer.getGameSpeed(); //La materia Klein si muove al contrario
        else */           return m_speed;
    }

    const std::vector<Hitbox*>& Entity::getHitboxes() const { return m_hitboxes; }

    /*Setters*/

    void Entity::setPosition(point_t pos)       { m_position      = pos;   }
    void Entity::setSpeed(vector_t vel)         { m_speed         = vel;   }
    void Entity::setMass(float m)               { m_mass          = m;     }
    void Entity::setKleiness(bool is_klein)     { m_is_klein      = is_klein;}

    void Entity::setFaction(int faction)
    {
        m_faction = faction;
        for (Hitbox* hb : m_hitboxes)
            hb->setFaction(faction);
    }


    void Entity::setTimeDirection(int dir)
    {
        s_mutex.lock();
        s_timeDirection = dir;
        s_mutex.unlock();
    }


    void Entity::addHitbox(Hitbox* hb) { KLEIN_ASSERT(hb != nullptr); m_hitboxes.push_back(hb); }


    void Entity::addSpeedContribution(vector_t impulse)
    {
        //printf("addSpeedContribution: m_next_speed pre-incremento:{%f, %f}, ", m_next_speed.x, m_next_speed.y);
        m_next_speed += impulse;
        //printf("m_next_speed post-incremento:{%f, %f}\n", m_next_speed.x, m_next_speed.y);
    }


    void Entity::updateMotion()
    {
        long dt = g_timer.getGameDelta();
        //printf("updateMotion: dt:%ld, ", dt);
        if(m_is_klein) dt = std::abs(dt);
        //printf("dt post-if:%ld, ", dt);
        //printf("m_speed pre-incremento:{%f, %f}, ", m_speed.x, m_speed.y);
        m_speed       += m_next_speed;
        //printf("m_speed post-incremento:{%f, %f}\n", m_speed.x, m_speed.y);
        m_position.x  += m_speed.x * dt * 1e-9;
        m_position.y  += m_speed.y * dt * 1e-9;
        m_next_speed = {0.f, 0.f};
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
        const float now = g_timer.getGameTime();

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