#pragma once

#include "Klein.h"
#include "Hitbox.h"
#include <vector>

namespace Klein
{
    /**
     * @brief Cella della griglia spaziale, contiene i riferimenti alle hitbox che la occupano.
     */
    class Cell
    {
    public:
        explicit Cell(int id = 0);
        ~Cell() = default;

        void addHitbox(Hitbox* h);

        void clear();   //Svuota la lista di hitbox della cella.

        void setCellID(int id);

        const int                     getCellID()     const; 
        const std::vector<Hitbox*>&   getHitboxes()   const;

    private:
        int                   m_id = 0;     //Id specifico della cella
        std::vector<Hitbox*>  m_hitboxes;   //Lista di hitbox presenti in questa cella
    };

    /**
     * @brief Griglia spaziale uniforme per la collision detection.
     *
     * Lo schermo viene suddiviso in celle di dimensione 'CELL_SIZE' x 'CELL_SIZE'.
     * Ogni hitbox viene assegnata alla cella corrispondente alla sua posizione,
     * riducendo il numero di confronti a quelli delle 8 celle più vicine
     */
    class Grid
    {
    public:
        Grid();
        ~Grid() = default;

        /*Assegna automaticamente una hitbox alla cella corrispondente in base alla sua posizione*/
        void assignHitboxToCell(Hitbox* h);

        /*Svuota la cella della griglia, da fare all'inizio di ogni frame*/
        void clearCell(int id);
        void clearAllCells();

        /*Converte coordinate di riga/colonna in un ID cella lineare*/
        int convertToID(int row, int col) const;

        const std::vector<Hitbox*>& getHitboxesAt(int row, int col)   const; 
        const std::vector<Hitbox*>& getHitboxesAt(int id)             const;           

        /*Getters*/

        const int getRows()         const;
        const int getCols()         const;
        const int getCellCount()    const;

        
        static constexpr int CELL_SIZE = MAX_PARTICLE_SIZE; // Dimensione di ogni cella, pari a 'MAX_PARTICLE_SIZE'

    private:
        std::vector<Cell> m_cells;  // Lista di tutte le celle

        int m_rows  = 0;  // Numero di righe della griglia
        int m_cols  = 0;  // Numero di colonne della griglia
    };

} //namespace Klein