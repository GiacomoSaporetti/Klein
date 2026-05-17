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
        printf("Creating wall at [t:%.f, b:%.f, l:%.f, r:%.f]\n", wall.top, wall.bottom, wall.left, wall.right);
        Entity* e = new Entity();
        e->setPosition({wall.origin()});
        e->setUnmovable(true);
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


    /*Costruttore*/
    Entity::Entity()  : m_timeOfBirth(g_timer.getGameTime()), m_is_klein(false){}
    
    Entity::~Entity()
    {
        for(auto h : this->m_hitboxes)
            delete h;
    }

    /*Getters*/

    point_t      Entity::getPosition()      const   { return m_position; }
    float        Entity::getMass()          const   { return m_mass; }
    float        Entity::getDissipation()   const   { return m_dissipation; }
    int          Entity::getHp()            const   { return m_hp; }
    int          Entity::getFaction()       const   { return m_faction; }
    float        Entity::getTimeOfBirth()   const   { return m_timeOfBirth; }
    vector_t     Entity::getSpeed()         const   { return m_speed; }
    bool         Entity::isUnmovable()      const   { return m_is_unmovable; }
    const std::vector<Hitbox*>& Entity::getHitboxes() const { return m_hitboxes; }

    /*Setters*/

    void Entity::setPosition(point_t pos)       { m_position      = pos;   }
    void Entity::setPosition(float x, float y)  { m_position      = {x, y};   }
    void Entity::setSpeed(vector_t vel)         { m_speed         = vel;   }
    void Entity::setMass(float m)               { m_mass          = m;     }
    void Entity::setKleiness(bool is_klein)     { m_is_klein      = is_klein;}
    void Entity::setUnmovable(bool is_unmovable){ m_is_unmovable  = is_unmovable;}

    void Entity::setFaction(int faction)
    {
        m_faction = faction;
        for (Hitbox* hb : m_hitboxes)
            hb->setFaction(faction);
    }


    void Entity::addHitbox(Hitbox* hb) { KLEIN_ASSERT(hb != nullptr); m_hitboxes.push_back(hb); }


    void Entity::addSpeedContribution(vector_t impulse)
    {
        m_next_speed += impulse;
        m_has_collided = true;
    }


    /**
     * Questo viene chiamato automaticamente per tutte le entità
     * dal g_timer quando viene eseguito TimeHandler::setGameSpeed
    */
    void Entity::handleTimeSpeedChange(float old_spd, float new_spd)
    {
        if(!m_is_klein) return;
        m_speed = m_speed*(new_spd/old_spd);
    }

    void Entity::updateMotion()
    {
        /*Se è inamovibile non la sposto*/
        if(m_is_unmovable) return;

        long dt = std::abs(g_timer.getGameDelta());

        /*Se non ho fatto collisioni skippo, altrimenti annullerei la velocità*/
        if(m_has_collided)  m_speed = m_next_speed;
        
        m_position.x  += m_speed.x * dt * 1e-9;
        m_position.y  += m_speed.y * dt * 1e-9;
        m_next_speed = {0.f, 0.f};
        m_has_collided = false;
    }


    void Entity::tick()
    {
        if(m_hp <= 0)
        {
            goInDeathState();
            return;
        }
        if(g_timer.getGameTime() < m_timeOfBirth)
        {
            vanish();
        }
        updateMotion();
    }


    void Entity::goInDeathState()
    {
        long now = g_timer.getGameTime();

        /*Prima volta che entro nel loop di morte*/
        if(!m_is_dead)  
        {
            m_is_dead = true;
            m_timeOfDeath = now;
        }

        // Resurrezione tramite inversione temporale
        /*Che succede */
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

    /**
     * @brief Un' entità svanisce quando il tempo di gioco precede il
     * momento della sua nascita
     */
    void Entity::vanish()
    {

    }

} //namespace Klein