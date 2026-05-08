#pragma once

#include "Klein.h"
#include "Entity.h"
#include "Grid.h"
#include <vector>
#include <chrono>
#include <iostream>



namespace Klein
{

    /**
     * @brief Coppia di hitbox che hanno colliduto.
     */
    struct CollisionCouple
    {
        Hitbox* first  = nullptr;
        Hitbox* second = nullptr;
    };


    /**
     * @brief Gestisce la rilevazione delle collisioni tra entità.
     *
     * Supporta tre implementazioni: naive O(n²), ottimizzazione per quadranti,
     * e ottimizzazione a griglia spaziale uniforme.
     */
    class CollisionHandler
    {
    public:
        CollisionHandler();
        ~CollisionHandler() = default;

        /// @brief Imposta la lista di entità su cui operare.
        void setEntitiesList(std::vector<Entity*>& entities);

        /// @brief Confronto naive O(n²) tra tutte le coppie di hitbox.
        int runNaiveImplementation();

        /// @brief Ottimizzazione con suddivisione in quadranti.
        int runQuadrantOptimization();

        /// @brief Ottimizzazione con griglia spaziale uniforme.
        int runGridOptimization();

        /// @brief Svuota la lista collisioni di tutte le entità.
        void clearEntitiesCollisionList();

        /// @brief Popola la lista interna di hitbox a partire dalle entità.
        int populateHitboxesList();

        /// @brief Azzera la velocità accumulata di tutte le entità.
        void resetMotion();

        /// @brief Aggiorna il movimento di tutte le entità.
        void updateMotion();

    private:
        std::vector<Entity*>         m_entities;
        std::vector<Hitbox*>         m_hitboxes;
        std::vector<CollisionCouple> m_collisions;
        Grid                         m_grid;

        /// @brief Stampa la lista delle collisioni rilevate (debug).
        void printCollisionList();
    };

} // namespace Klein