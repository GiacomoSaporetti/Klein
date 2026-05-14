#pragma once

#include "Klein.h"
#include "Entity.h"
#include "TimeHandler.h"
#include "Grid.h"
#include <vector>
#include <chrono>
#include <iostream>




namespace Klein
{

    extern std::vector<Entity*>     g_all_entities;  //Lista globale delle entità
    extern TimeHandler              g_timer;         //Timer globale

    /**
     * @brief Coppia di hitbox che hanno colliso.
     */
    struct CollisionCouple
    {
        Entity* first     = nullptr;
        Hitbox* first_hb  = nullptr;
        Entity* second    = nullptr;
        Hitbox* second_hb = nullptr;
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
        void stopMotion();

        void removeInactiveCollisions();

        bool areAlreadyColliding(Entity* h1, Entity* h2);

        bool areOverlapping(Hitbox* h1, Hitbox* h2);
        bool areCirclesOverlapping(Hitbox* h1, Hitbox* h2);
        bool checkCollision(Hitbox* h1, Hitbox* h2);
        int  countCollisions(std::vector<Hitbox*>& list);
        void computeCollision(Hitbox* e1, Hitbox* e2);
        void checkCollisionAgainstWalls();

    private:
        std::vector<Hitbox*>         m_hitboxes;
        std::vector<Hitbox*>         m_neighbourhood;   //Evito di allocare un vettore ogni volta
        std::vector<CollisionCouple> m_collisions;
        Grid                         m_grid;


        void gatherNeighbourhood(Grid& grid, int col, int row);
        void countGridMultithread(Grid& grid, int startCol, int stopCol, int startRow, int stopRow, int& res);
        void printCollisionList();
    };

} // namespace Klein