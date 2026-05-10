#include "CollisionHandler.h"

// -------------------------------------------------------------------------
// Debug flags
// -------------------------------------------------------------------------

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
    static bool areOverlapping(Hitbox* h1, Hitbox* h2);
    static bool checkDistanceWithinRadius(Hitbox* h1, Hitbox* h2);
    static bool checkCollision(Hitbox* h1, Hitbox* h2);
    static int  countCollisions(std::vector<Hitbox*>& list);
    static void countCollisionsThread(std::vector<Hitbox*>& list, int& res);
    static void computeCollision(Entity* e1, Entity* e2);
    static void populateCells(Grid& grid, std::vector<Hitbox*>& list, int start, int stop);
    static void countGridMultithread(Grid& grid, int startCol, int stopCol, int startRow, int stopRow, int& res);


    /**
     * @brief Calcola e applica le velocità post-collisione a due entità.
     *
     * Conserva la quantità di moto scalata per il fattore di dissipazione.
     */
    static void computeCollision(Entity* e1, Entity* e2)
    {
        KLEIN_ASSERT(e1 != nullptr && e2 != nullptr);

 
        // Coefficiente di restituzione: 0 = anelastica totale, 1 = elastica
        const float   = e1->getDissipation() * e2->getDissipation() / 2.f;

        // Normale d'impatto (da e1 verso e2), normalizzata
        const point_t  p1 = e1->getPosition(), p2 = e2->getPosition();
        vector_t n = { p2.x - p1.x, p2.y - p1.y };
        n = n.normalized();

        // Componenti di velocità lungo la normale
        const vector_t v1 = e1->getSpeed(), v2 = e2->getSpeed();
        const float u1 = v1.x * n.x + v1.y * n.y;
        const float u2 = v2.x * n.x + v2.y * n.y;

        // Le particelle si stanno già allontanando: niente impulso
        //if (u1 - u2 <= 0.f) return;

        const float m1 = e1->getMass(), m2 = e2->getMass();
        const float M  = m1 + m2;

        // Velocità post-collisione lungo la normale
        const float u1p = (m1*u1 + m2*u2 - m2 * e * (u1 - u2)) / M;
        const float u2p = (m1*u1 + m2*u2 + m1 * e * (u1 - u2)) / M;

        // Applica solo la variazione lungo n (la componente perpendicolare resta invariata)
        e1->setSpeed({ v1.x + (u1p - u1) * n.x,  v1.y + (u1p - u1) * n.y });
        e2->setSpeed({ v2.x + (u2p - u2) * n.x,  v2.y + (u2p - u2) * n.y });

        // Separazione posizionale — spinge le entità fuori dalla sovrapposizione
        // Usa getRadius() della hitbox, non la posizione diretta
        const float r1      = e1->getHitboxes()[0]->getRadius();
        const float r2      = e2->getHitboxes()[0]->getRadius();
        const float overlap = (r1 + r2) - d;   // d è già calcolata sopra

        if (overlap > 0.f)
        {
            const float push = overlap / 2.f;
            e1->setPosition({ p1.x - n.x * push, p1.y - n.y * push });
            e2->setPosition({ p2.x + n.x * push, p2.y + n.y * push });
        }
    }


    static bool checkDistanceWithinRadius(Hitbox* h1, Hitbox* h2)
    {
        const point_t  p1 = h1->getCenter();
        const point_t  p2 = h2->getCenter();
        const vector_t d  = { static_cast<float>(p1.x - p2.x),
                               static_cast<float>(p1.y - p2.y) };
        return d.magnitude() < (h1->getRadius() + h2->getRadius());
    }

    /**
     * @brief Esegue dei controlli prima di verificare 
     * l'effettiva sovrapposizione geometrica delle figure con 'areOverlapping'
     */
    static bool checkCollision(Hitbox* h1, Hitbox* h2)
    {
        Entity* e1 = &h1->getParentEntity();
        Entity* e2 = &h2->getParentEntity();

        if (e1 == e2)                   return false;
        if (e2->hasAlreadyCollided(e1)) return false;

        #ifdef FACTIONS
            if (h1->getFaction() == h2->getFaction()) return false;
        #endif

        return areOverlapping(h1, h2);
    }

    /**
     * @brief Controlla se due hitbox si stanno ancora sovrapponendo
     *         (stessa logica di checkCollision ma senza controllo stato)
    */
    static bool areOverlapping(Hitbox* h1, Hitbox* h2)
    {
        const HitboxType t1 = h1->getType();
        const HitboxType t2 = h2->getType();

  

        /*Caso Cerchio-Cerchio*/
        if (t1 == HitboxType::Circle && t2 == HitboxType::Circle)    
            return checkDistanceWithinRadius(h1, h2);


        /*Almeno uno è un rettangolo, quindi verifico se è impossibile che le aree esterne siano in collisione*/
        const rectangle_t a1 = h1->getArea();
        const rectangle_t a2 = h2->getArea();

        /*
        std::cout << "HITBOXES"
        << "h1 | top=" << a1.top       << " bottom=" << a1.bottom
        << "h2 | top=" << a2.top       << " bottom=" << a2.bottom
        << '\n';
        */
        if (a1.top    > a2.bottom) return false;
        if (a2.top    > a1.bottom) return false;
        if (a1.right  < a2.left)   return false;
        if (a2.right  < a1.left)   return false;

        if(t1 == HitboxType::Rectangle && t2 == HitboxType::Rectangle)  return true;

        /*A questo punto uno dei due è Cerchio e l'altro è rettangolo*/
        if (t1 != t2)
        {
            /*Identifico quale è il cerchio e quale il rettangolo*/
            Hitbox* circleHb = (t1 == HitboxType::Circle) ? h1 : h2;
            Hitbox* rectHb   = (t1 == HitboxType::Circle) ? h2 : h1;

            const point_t  c = circleHb->getCenter();
            const float    r = circleHb->getRadius();
            const rectangle_t rect = rectHb->getArea();

            /*Punto del rettangolo più vicino al centro del cerchio*/
            const float nearestX = std::clamp(c.x, rect.left, rect.right);
            const float nearestY = std::clamp(c.y, rect.bottom, rect.top);

            const float dx = c.x - nearestX;
            const float dy = c.y - nearestY;
            return (dx*dx + dy*dy) <= (r*r);
        }

        return true;
    }


    static int countCollisions(std::vector<Hitbox*>& list)
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
                if (checkEntity->hasAlreadyCollided(activeEntity)) continue;

                if (areOverlapping(active, check))
                {
                    //printf("Overlap di %p e %p\n", activeEntity, checkEntity);
                    activeEntity->addCollided(checkEntity);
                    checkEntity->addCollided(activeEntity);
                    computeCollision(activeEntity, checkEntity);
                    count++;
                }
            }
        }
        return count;
}


    static void countCollisionsThread(std::vector<Hitbox*>& list, int& res)
    {
        res = countCollisions(list);
    }


    static void populateCells(Grid& grid, std::vector<Hitbox*>& list, int start, int stop)
    {
        for (int i = start; i < stop && i < static_cast<int>(list.size()); i++)
        {
            if (list[i] != nullptr)
                grid.assignHitboxToCell(list[i]);
        }
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

    void CollisionHandler::countGridMultithread(Grid& grid, int startCol, int stopCol,
                                                 int startRow, int stopRow, int& res)
    {
        int count = 0;
        for (int col = startCol; col <= stopCol; col += 2)
        for (int row = startRow; row <= stopRow; row += 2)
        {
            gatherNeighbourhood(grid, col, row);
            count += countCollisions(m_neighbourhood);
        }
        res = count;
    }


    CollisionHandler::CollisionHandler()
    {
        KLEIN_DEBUG("CollisionHandler", "initialized");
    }


    void CollisionHandler::setEntitiesList(std::vector<Entity*>& entities)
    {
        m_entities = entities;
    }

    int CollisionHandler::populateHitboxesList()
    {
        m_hitboxes.clear();
        for (Entity* e : m_entities)
            for (Hitbox* h : e->getHitboxes())
                m_hitboxes.push_back(h);
        return static_cast<int>(m_hitboxes.size());
    }



    void CollisionHandler::resetMotion()
    {
        for (Entity* e : m_entities)
            e->setAcceleration({0.f, 0.f});
    }

    void CollisionHandler::updateMotion()
    {
        for (Entity* e : m_entities)
            e->updateMotion();
    }

    void CollisionHandler::clearEntitiesCollisionList()
    {
        for (Entity* e : m_entities)
            e->clearCollidedList();
    }



    int CollisionHandler::runGridOptimization()
    {
        if (populateHitboxesList() == 0) return 0;

        for (Hitbox* h : m_hitboxes)
            m_grid.assignHitboxToCell(h);

        // DEBUG — stampa posizione entità vs centro hitbox per le prime 3 entità
        for (int i = 0; i < std::min(3, (int)m_entities.size()); i++)
        {
            Entity* e     = m_entities[i];
            point_t epos  = e->getPosition();
            point_t hpos  = e->getHitboxes()[0]->getCenter();
            float   hrad  = e->getHitboxes()[0]->getRadius();
            //printf("E[%d] pos=(%.1f,%.1f)  hb_center=(%.1f,%.1f)  radius=%.1f\n",
            //    i, epos.x, epos.y, hpos.x, hpos.y, hrad);
        }

        int count = 0;

        /*
        * Ottimizzazione: iterazione con passo 2
        *
        * Ogni iterazione raccoglie il "vicinato" della cella (col, row),
        * ovvero la cella stessa più le 8 celle adiacenti, e controlla
        * tutte le collisioni al suo interno.
        *
        * Poiché due celle qualsiasi distanti al più 1 passo sono sempre
        * entrambe incluse nel vicinato della prima, avanzare di 2 in
        * entrambe le direzioni garantisce che ogni coppia di celle
        * venga analizzata esattamente una volta, senza omissioni né
        * duplicati.
        *
        * È assolutemnte necessario che le hitbox non siano più grandi della 
        * cella stessa. Se viene superato questo limite, un'entità può sforare
        * oltre la cella adiacente e raggiungere una cella saltata,
        * causando collisioni non rilevate.
        */

        for (int col = 1; col <= m_grid.getCols(); col += 2)
            for (int row = 1; row <= m_grid.getRows(); row += 2)
            {
                m_neighbourhood.clear();  // non dealloca, mantiene la capacità
    
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
        clearEntitiesCollisionList();
        updateMotion();
        return count;
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