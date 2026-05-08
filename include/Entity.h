#pragma once

#include "Klein.h"
#include "TimeHandler.h"
#include "Hitbox.h"
#include <vector>
#include <semaphore.h>
#include <mutex>

namespace Klein
{
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

        void setAcceleration(vector_t acc);

        void setMass(float m);

        void setFaction(int faction);

        /*Imposta l'ID della cella in cui si trova*/
        void setCellID(int id);

        static void setTimer(TimeHandler* timer);

        static void setTimeDirection(int dir);
 


        void addHitbox(Hitbox& htbx);
        void addHitbox(Hitbox* htbx);

        void clearCollidedList();

        /*Registra una collisione avvenuta con 'other' nella lista comune*/
        void addCollided(Entity* other);

        /*Verifica se due entità hanno già colliduto durante questo frame*/
        bool hasAlreadyCollided(const Entity* other) const;

        /*Aggiorna il movimento dell'entità*/
        void updateMotion();

        void addActiveCollision(Entity* e)    { m_activeCollisions.push_back(e); }

        void removeActiveCollision(Entity* e)
        {
            m_activeCollisions.erase(
                std::remove(m_activeCollisions.begin(), m_activeCollisions.end(), e),
                m_activeCollisions.end());
        }
        bool isActivelyColliding(Entity* e) const
        {
            return std::find(m_activeCollisions.begin(),
                            m_activeCollisions.end(), e) != m_activeCollisions.end();
        }

    private:
        int     m_hp             = 50;      
        int     m_faction        = 0;       // Serve ad annullare il fuoco amico
        int     m_cellID         = INT_ERROR_VALUE;
        float   m_timeOfBirth    = -1.f;
        float   m_timeOfDeath    = -1.f;
        float   m_mass           = 1.f;
        float   m_dissipation    = 1.f;

        point_t  m_position      = {0.f, 0.f};
        vector_t m_speed         = {0.f, 0.f};
        vector_t m_acceleration  = {0.f, 0.f};

        std::vector<Hitbox*>   m_hitboxes;


        /*Comuni a tutte le istanze*/
        static std::mutex       s_mutex;                        // Per gestire la concorrenza a membri statici
        static int              s_timeDirection; 
        static TimeHandler*     s_timer;                        // Clock di gioco
        std::vector<Entity*>    m_currentFrameCollisionList;    // Collisioni del frame attuale (da resettare ogni frame)
        std::vector<Entity*>    m_activeCollisions;             // Collisioni recenti, non resettata


        /**
         * @brief Disabilita danni e collisioni senza distruggere l'istanza,
         *        per consentire la resurrezione tramite inversione temporale.
         */
        void goInDeathState();
    };

} // namespace Klein