#pragma once

#include <algorithm>
#include "Klein.h"
#include "TimeHandler.h"
#include <vector>
#include <semaphore.h>
#include <mutex>

namespace Klein
{
    class Hitbox;

        /*Soglie per stabilire il cambio di comportamento verso il giocatore*/

    inline constexpr int KARMA_THRESHOLD_LOWEST  = -1000;
    inline constexpr int KARMA_THRESHOLD_LOW     =  -500;
    inline constexpr int KARMA_THRESHOLD_MID     =     0;
    inline constexpr int KARMA_THRESHOLD_HIGH    =   500;
    inline constexpr int KARMA_THRESHOLD_HIGHEST =  1000;


    /**
     * @brief Livello di aggressività di un'entità nei confronti delle altre.
     */
    enum class AggroLevel
    {
        Protective, // Difende attivamente le entità alleate
        Friendly,   // Non attacca, interagisce positivamente
        Neutral,    // Ignora le altre entità
        Unfriendly, // Evita o minaccia senza attaccare
        Aggressive  // Attacca attivamente
    };


    /**
     * @brief Entità base 
     *
     * Il timer condiviso 'TIMER' deve essere impostato prima di istanziare
     * qualsiasi entità tramite 'Entity::setTimer()'
     */
    class Entity
    {
    public:
        Entity();
        ~Entity();

        /*Aggiorna lo stato dell'entità per il frame corrente*/
        void tick();

        /*Getters*/

        point_t   getPosition()      const;
        vector_t  getSpeed()         const;
        float     getMass()          const;
        float     getDissipation()   const;
        int       getHp()            const;
        int       getFaction()       const;
        int       getCellID()        const;
        float     getTimeOfBirth()   const;

        const std::vector<Hitbox*>& getHitboxes() const;

        static int getTimeDirection();

        /*Setters*/

        void setPosition(point_t pos);
        void setSpeed(vector_t vel);
        void setMass(float m);
        void setFaction(int faction);
        void setKleiness(bool is_klein);
        static void setTimeDirection(int dir);
 
        void addHitbox(Hitbox* htbx);
        void addSpeedContribution(vector_t impulse);
        /*Aggiorna il movimento dell'entità*/
        void updateMotion();

    private:
        int     m_hp             = 50;      
        int     m_faction        = 0;       // Serve ad annullare il fuoco amico
        float   m_timeOfBirth    = -1.f;
        float   m_timeOfDeath    = -1.f;
        float   m_mass           = 1.f;
        float   m_dissipation    = 0.5f;
        bool    m_is_klein       = false;
        point_t  m_position      = {0.f, 0.f};
        vector_t m_speed         = {0.f, 0.f};
        vector_t m_next_speed    = {0.f, 0.f};

        std::vector<Hitbox*>   m_hitboxes;


        /*Comuni a tutte le istanze*/
        static std::mutex       s_mutex;                        // Per gestire la concorrenza a membri statici
        static int              s_timeDirection; 
        bool m_ignore_time_direction = false;
        std::vector<Entity*>    m_currentFrameCollisionList;    // Collisioni del frame attuale (da resettare ogni frame)
        std::vector<Entity*>    m_activeCollisions;             // Collisioni recenti, non resettata


        /**
         * @brief Disabilita danni e collisioni senza distruggere l'istanza,
         *        per consentire la resurrezione tramite inversione temporale.
         */
        void goInDeathState();
    };


    void AddEntity(Entity* e);
    std::vector<Entity*>& GetAllEntities();
    Entity* CreateWall(rectangle_t wall);
} // namespace Klein