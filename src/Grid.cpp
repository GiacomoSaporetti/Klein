
#include "Grid.h"

namespace Klein
{
    /*Cell*/

    Cell::Cell(int id)  : m_id(id)  {}

    /*Metodi*/

    void Cell::addHitbox(Hitbox* h)  { m_hitboxes.push_back(h); }
    void Cell::clear()               { m_hitboxes.clear();      }
    void Cell::setCellID(int id)     { m_id = id;               }

    /*Getters*/
    
    int                         Cell::getCellID()   const { return m_id;       }
    const std::vector<Hitbox*>& Cell::getHitboxes() const { return m_hitboxes; }
    
    

    /*Grid*/

    Grid::Grid()
    {
        m_rows = SCREEN_HEIGHT / CELL_SIZE + (SCREEN_HEIGHT % CELL_SIZE != 0 ? 1 : 0);
        m_cols = SCREEN_WIDTH  / CELL_SIZE + (SCREEN_WIDTH  % CELL_SIZE != 0 ? 1 : 0);

        m_cells.reserve(m_rows * m_cols);
        for (int i = 0; i < m_rows * m_cols; i++)
            m_cells.emplace_back(i);

        KLEIN_DEBUG("Grid rows:",  m_rows);
        KLEIN_DEBUG("Grid cols:",  m_cols);
        KLEIN_DEBUG("Cell size:",  MAX_PARTICLE_SIZE);
    }

    /*Getters*/

    int Grid::getRows()      const { return m_rows;           }
    int Grid::getCols()      const { return m_cols;           }
    int Grid::getCellCount() const { return m_rows * m_cols;  }
    
    const std::vector<Hitbox*>& Grid::getHitboxesAt(int row, int col) const
    {
        return m_cells[convertToID(row, col)].getHitboxes();
    }

    const std::vector<Hitbox*>& Grid::getHitboxesAt(int id) const
    {
        KLEIN_ASSERT(id >= 0 && id < static_cast<int>(m_cells.size()));
        return m_cells[id].getHitboxes();
    }

    /*Metodi*/

    void Grid::assignHitboxToCell(Hitbox* h)
    {
        KLEIN_ASSERT(h != nullptr);
        const point_t center = h->getCenter();
        const int row = center.y / CELL_SIZE;
        const int col = center.x / CELL_SIZE;
        
        // Scarta hitbox fuori dalla griglia
        if (row < 0 || row >= m_rows) return;
        if (col < 0 || col >= m_cols) return;
        
        const int id  = convertToID(row, col);
        if(id<0) return;
        h->setCellID(id);
        m_cells[id].addHitbox(h);
    }

    int Grid::convertToID(int row, int col) const
    {
        //KLEIN_ASSERT(row >= 0 && row < m_rows);
        //KLEIN_ASSERT(col >= 0 && col < m_cols);
        return row * m_cols + col;
    }

    void Grid::clearCell(int id)
    {
        KLEIN_ASSERT(id >= 0 && id < static_cast<int>(m_cells.size()));
        m_cells[id].clear();
    }

    void Grid::clearAllCells()
    {
        for(int id=0; id< m_rows*m_cols; id++)
            m_cells[id].clear();
    }

} //namespace Klein