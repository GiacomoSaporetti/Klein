#include "CollisionHandler.h"


//#define PRINT_COLLISION
//#define MEASURE_TIME
//#define FACTIONS

#ifdef PRINT_COLLISION
    #define PRINT_COLLISION_LIST printCollisionList()
#else
    #define PRINT_COLLISION_LIST
#endif


namespace Klein
{
    CollisionHandler    g_collision_handler;


    CollisionHandler::CollisionHandler()
    {
        m_collisions.reserve(10);
        m_collisions.clear();
        KLEIN_DEBUG("CollisionHandler", "initialized");
    }



    /**
     * @brief Calcola e applica le velocità post-collisione a due entità.
     *
     * Conserva la quantità di moto scalata per il fattore di dissipazione.
     */
    void CollisionHandler::computeCollision(Entity* e1, Entity* e2)
    {
        KLEIN_ASSERT(e1 != nullptr && e2 != nullptr);
 
        
        Hitbox* ha = e1->getHitboxes()[0];
        Hitbox* hb = e2->getHitboxes()[0];
        std::cout   << "COLLISIONE!\n"
                    << "  A=("  << e1->getPosition().x << "," << e1->getPosition().y << ")"
                    << "  ["  << ha->getBoundingBox().left << "," << ha->getBoundingBox().right << "]"
                    << "  vA=(" << e1->getSpeed().x    << "," << e1->getSpeed().y    << ")"
                    << "  B=("  << e2->getPosition().x << "," << e2->getPosition().y << ")"
                    << "  ["  << hb->getBoundingBox().left << "," << hb->getBoundingBox().right << "]"
                    << "  vB=(" << e2->getSpeed().x    << "," << e2->getSpeed().y    << ")"
                    << '\n';
        
        // Coefficiente di restituzione: 0 = anelastica totale, 1 = elastica
        const float e  = e1->getDissipation() * e2->getDissipation();


        // Normale d'impatto (da e1 verso e2), normalizzata
        const point_t  p1 = e1->getPosition(), p2 = e2->getPosition();
        vector_t n = { p2.x - p1.x, p2.y - p1.y };
        n = n.normalized();

        // Componenti di velocità lungo la normale
        const vector_t v1 = e1->getSpeed();
        const vector_t v2 = e2->getSpeed();
        const float u1 = v1 * n;
        const float u2 = v2 * n;


        const float m1 = e1->getMass();
        const float m2 = e2->getMass();
        const float M  = m1 + m2;

        // Velocità post-collisione lungo la normale
        const float u1p = (m1*u1 + m2*u2 - m2 * e * (u1 - u2)) / M;
        const float u2p = (m1*u1 + m2*u2 + m1 * e * (u1 - u2)) / M;


        const vector_t dv1 = n * (u1p - u1);
        const vector_t dv2 = n * (u2p - u2);

        std::cout   << "dv1: {" << dv1.x << ", " << dv1.y << "}  "
                    << "dv2: {" << dv2.x << ", " << dv2.y << "}\n";

        e1->addSpeedContribution(dv1);
        e2->addSpeedContribution(dv2);
    }


    /**
     * @brief Varifica la collisione tra 2 cerchi
     */
    bool CollisionHandler::areCirclesOverlapping(Hitbox* h1, Hitbox* h2)
    {
        const point_t  p1 = h1->getCenter();
        const point_t  p2 = h2->getCenter();
        const vector_t d  = { p1.x - p2.x, p1.y - p2.y};
        return d.magnitude() < (h1->getRadius() + h2->getRadius());
    }

    /**
     * @brief Esegue dei controlli prima di verificare 
     * l'effettiva sovrapposizione geometrica delle figure con 'areOverlapping'
     */
    bool CollisionHandler::checkCollision(Hitbox* h1, Hitbox* h2)
    {
        //printf("Checking collision between %p, %p\n", h1, h2);
        if(areAlreadyColliding(h1, h2)) return false;

        Entity* e1 = &h1->getParentEntity();
        Entity* e2 = &h2->getParentEntity();

        if (e1 == e2)                   return false;   //Hitbox della stessa entità non collidono

        #ifdef FACTIONS
            if (h1->getFaction() == h2->getFaction()) return false;
        #endif

        return areOverlapping(h1, h2);
    }


    /**
     * @brief Controlla se due hitbox si stanno ancora sovrapponendo
    */
    bool CollisionHandler::areOverlapping(Hitbox* h1, Hitbox* h2)
    {
        const HitboxType t1 = h1->getType();
        const HitboxType t2 = h2->getType();

  
        /*Caso Cerchio-Cerchio*/
        if (t1 == HitboxType::Circle && t2 == HitboxType::Circle)    
            return areCirclesOverlapping(h1, h2);

        /*Almeno uno è un rettangolo, quindi verifico se è impossibile che le aree esterne siano in collisione*/
        const rectangle_t a1 = h1->getBoundingBox();
        const rectangle_t a2 = h2->getBoundingBox();

        if (a1.top    > a2.bottom) return false;
        if (a2.top    > a1.bottom) return false;
        if (a1.right  < a2.left)   return false;
        if (a2.right  < a1.left)   return false;

        if(t1 == HitboxType::Rectangle && t2 == HitboxType::Rectangle)  return true;

        /*A questo punto uno dei due è Cerchio e l'altro è rettangolo*/

        /*Identifico quale è il cerchio e quale il rettangolo*/
        Hitbox* circleHb = (t1 == HitboxType::Circle) ? h1 : h2;
        Hitbox* rectHb   = (t1 == HitboxType::Circle) ? h2 : h1;

        const point_t  c = circleHb->getCenter();
        const float    r = circleHb->getRadius();
        const rectangle_t rect = rectHb->getBoundingBox();

        /*Punto del rettangolo più vicino al centro del cerchio*/
        const float nearestX = std::clamp(c.x, rect.left, rect.right);
        const float nearestY = std::clamp(c.y, rect.bottom, rect.top);

        const float dx = c.x - nearestX;
        const float dy = c.y - nearestY;
        return (dx*dx + dy*dy) <= (r*r);
    }


    int CollisionHandler::countCollisions(std::vector<Hitbox*>& list)
    {
        int count = 0;
    
        for (int i = 0; i < static_cast<int>(list.size()); i++)
        {
            Hitbox* active       = list[i];
            Entity* activeEntity = &active->getParentEntity();

            for (int j = i + 1; j < static_cast<int>(list.size()); j++)
            {
                Hitbox* check       = list[j];
                Entity* checkEntity = &check->getParentEntity();

                if (activeEntity == checkEntity)               continue;

                if (checkCollision(active, check))
                {
                    m_collisions.push_back({active, check});
                    computeCollision(activeEntity, checkEntity);
                    count++;
                }
            }
        }
        return count;
    }


    /** 
     * @brief Raccoglie le hitbox delle 9 celle attorno a (col, row) e popola il vettore m_neighbourhood
     */
    void CollisionHandler::gatherNeighbourhood(Grid& grid, int col, int row)
    {
        m_neighbourhood.clear();
        for (int dc = -1; dc <= 1; dc++)
        for (int dr = -1; dr <= 1; dr++)
        {
            const int c = col + dc;
            const int r = row + dr;
            if (c < 0 || c >= grid.getCols()) continue;
            if (r < 0 || r >= grid.getRows()) continue;
            const auto& cell = grid.getHitboxesAt(r, c);
            m_neighbourhood.insert(m_neighbourhood.end(), cell.begin(), cell.end());
        }
    }


    int CollisionHandler::populateHitboxesList()
    {
        m_hitboxes.clear();
        for (Entity* e : g_all_entities)
            for (Hitbox* h : e->getHitboxes())
                m_hitboxes.push_back(h);
        return m_hitboxes.size();
    }


    int CollisionHandler::runGridOptimization()
    {
        if (populateHitboxesList() == 0) return 0;

        for (Hitbox* h : m_hitboxes)    m_grid.assignHitboxToCell(h);

        int count = 0;

        /*
        * Ottimizzazione: passo 2
        *
        * Ogni iterazione considera una cella e le 8 adiacenti,
        * controllando tutte le collisioni locali.
        *
        * Il passo 2 evita duplicati: ogni coppia viene analizzata una sola volta.
        *
        * Richiede hitbox ≤ dimensione della cella, altrimenti si perdono collisioni.
        */

        for (int col = 1; col <= m_grid.getCols(); col += 2)
            for (int row = 1; row <= m_grid.getRows(); row += 2)
            {
                m_neighbourhood.clear();
    
                for (int dc = -1; dc <= 1; dc++)
                for (int dr = -1; dr <= 1; dr++)
                {
                    const int c = col + dc;
                    const int r = row + dr;
                    if (c < 0 || c >= m_grid.getCols()) continue;
                    if (r < 0 || r >= m_grid.getRows()) continue;
                    const auto& cell = m_grid.getHitboxesAt(r, c);
                    m_neighbourhood.insert(m_neighbourhood.end(), cell.begin(), cell.end());
                }
                count += countCollisions(m_neighbourhood);
            }

        m_grid.clearAllCells();

        PRINT_COLLISION_LIST;
        return count;
    }


    bool CollisionHandler::areAlreadyColliding(Hitbox* h1, Hitbox* h2)
    {
        for(CollisionCouple couple : m_collisions)
        {
            if(h1 == couple.first && h2 == couple.second ||
                h2 == couple.second && h1 == couple.first)
                    return true;
        }
        return false;
    }

    
    void CollisionHandler::removeInactiveCollisions()
    {
        auto it = m_collisions.begin();
        while (it != m_collisions.end())
        {
            if (!areOverlapping(it->first, it->second))
            {
                std::swap(*it, m_collisions.back());
                m_collisions.pop_back();
            }
            else
                it++;
        }
    }

    // -------------------------------------------------------------------------
    // Debug
    // -------------------------------------------------------------------------

    void CollisionHandler::printCollisionList()
    {
        for (const CollisionCouple& couple : m_collisions)
        {
            const Hitbox* h1 = couple.first;
            const Hitbox* h2 = couple.second;

            std::cout << h1 << " | "
                      << "c= " << h1->getCenter().x << ", " << h1->getCenter().y << " | "
                      << "t= " << static_cast<int>(h1->getType())
                      << " r= " << h1->getRadius()
                      << " h= " << h1->getHeight()
                      << " w= " << h1->getWidth()
                      << " cell= " << h1->getCellID() << " / "
                      << h2 << " | "
                      << "c= " << h2->getCenter().x << ", " << h2->getCenter().y << " | "
                      << "t= " << static_cast<int>(h2->getType())
                      << " r= " << h2->getRadius()
                      << " h= " << h2->getHeight()
                      << " w= " << h2->getWidth()
                      << " cell= " << h2->getCellID() << '\n';
        }
        m_collisions.clear();
    }


} // namespace Klein