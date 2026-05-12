#pragma once

#include "Klein.h"

namespace Klein
{
    /*Forward declaration per evitare include circolare*/
    class Entity;

    /**
     * @brief Volume di collisione.
     * Può avere diverse geometrie (vedere HitboxType). 
     */
    class Hitbox
    {
    public:
        /*Costruttore per hitbox circolare*/
        Hitbox(Entity& parent, const circle_t& shape);

        /*Costruttore per hitbox rettangolare*/
        Hitbox(Entity& parent, const rectangle_t& shape);

        ~Hitbox() = default;

        /*Aggiorna m_area (utile specialmente per i cerchi)*/
        void updateAABB();

        /*Getters*/

        HitboxType  getType()           const;
        point_t     getCenter()         const;
        float       getRadius()         const;
        int         getWidth()          const;
        int         getHeight()         const;
        int         getFaction()        const;
        float       getMass()           const;
        int         getCellID()         const;

        /**
        @brief Fornisce i bordi della hitbox in termini assoluti rispetto alle coordinate dello schermo
        */
        rectangle_t getBoundingBox()           const;
        Entity&     getParentEntity()   const;
        
        
        /*Setters*/

        /*Solo per cerchi*/
        void setRadius(float r);

        /*Solo per rettangoli*/
        void setWidth(int w);
        void setHeight(int h);

        void setFaction(int faction);
        void setMass(float mass);
        void setCellID(int id);

    private:
        HitboxType  m_type;
       
        point_t     m_offset  = {0, 0}; // Offset del centro rispetto alla posizione del parent

        float       m_radius  = 0.f;
        int         m_width   = 0;
        int         m_height  = 0;
        int         m_faction = 0;
        float       m_mass    = 0.f;
        int         m_cellID  = 0;
        rectangle_t m_area    = {}; // AABB in coordinate relative (senza posizione assoluta del parent)
        Entity&     m_parent;
    };

} //namespace Klein