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
     * Deve ragionare in termini assoluti, e vedere le particelle che si muovono nel tempo reale
     */
    void CollisionHandler::computeCollision(Hitbox* h1, Hitbox* h2)
    {
        KLEIN_ASSERT(h1 != nullptr && h2 != nullptr);
        Entity* e1 = &h1->getParentEntity();
        Entity* e2 = &h2->getParentEntity();
        KLEIN_ASSERT(e1 != nullptr && e2 != nullptr);

        std::cout   << "COLLISIONE!\n"
                    << "  E1=("  << e1->getPosition().x << "," << e1->getPosition().y << ")"
                    << "  ["  << h1->getBoundingBox().top << "," << h1->getBoundingBox().bottom <<  "," << h1->getBoundingBox().left << "," << h1->getBoundingBox().right << "]"
                    << "  ve1=(" << e1->getSpeed().x    << "," << e1->getSpeed().y    << ")"
                    << "  E2=("  << e2->getPosition().x << "," << e2->getPosition().y << ")"
                    << "  ["  << h2->getBoundingBox().top << "," << h2->getBoundingBox().bottom <<  "," << h2->getBoundingBox().left << "," << h2->getBoundingBox().right << "]"
                    << "  ve2=(" << e2->getSpeed().x    << "," << e2->getSpeed().y    << ")"
                    << '\n';
        
        float dissipation  = e1->getDissipation() * e2->getDissipation();

        /*Normale d'impatto (da e1 verso e2), normalizzata*/
        vector_t v1 = e1->getSpeed();
        vector_t v2 = e2->getSpeed();

        /*
            Bisogna ora capire come calcolare la normale dell'impatto.
            Se sono entrambi cerchi allora si puo usare il loro centro,
            ma se uno dei due è rettangolo allora bisogna trovare il punto del suo
            perimetro più vicino al centro dell'altra hitbox. 
        */
        vector_t n = {0.f, 0.f};

        /*Caso in cui sono entrambi cerchi*/
        if(h1->getType() == HitboxType::Circle && h2->getType() == HitboxType::Circle)
        {
            point_t  p1 = h1->getCenter(); 
            point_t  p2 = h2->getCenter();
            n  = { p2.x - p1.x, p2.y - p1.y };
            n = n.normalized();
        }
        /*Almeno uno dei due è rettangolo*/
        else
        {
            Hitbox* rectHb  = (h2->getType() == HitboxType::Rectangle) ? h2 : h1;
            Hitbox* otherHb = (rectHb == h2) ? h1 : h2;

            const point_t     c    = otherHb->getCenter();
            const rectangle_t rect = rectHb->getBoundingBox();

            const float nearestX = std::clamp(c.x, rect.left, rect.right);
            const float nearestY = std::clamp(c.y, rect.top, rect.bottom);

            const float dx = c.x - nearestX;
            const float dy = c.y - nearestY;

            // n punta da rectHb verso otherHb
            n = vector_t{ dx, dy }.normalized();

            // Se il rettangolo è h1, n punta da h1 a h2: corretto (convenzione uguale al caso cerchio-cerchio)
            // Se il rettangolo è h2, n punta da h2 a h1: va invertita
            if(rectHb == h2) n = n * -1.f;
        }

        /*Componenti delle velocità normali all'impatto*/
        vector_t v1n = n * (v1 * n);
        vector_t v2n = n * (v2 * n);

        /*Componenti tangenziali delle velocità*/
        vector_t v1t = v1 - v1n;
        vector_t v2t = v2 - v2n;

        /*Scalari delle componenti normali*/
        float v1n_scalar = v1 * n;
        float v2n_scalar = v2 * n;

        vector_t new_v1, new_v2;

        
        // --- DEBUG ---
        std::cout << "  n = {" << n.x << ", " << n.y << "}\n";
        std::cout << "  v1n_scalar=" << v1n_scalar << "  v2n_scalar=" << v2n_scalar << "\n";
        std::cout << "  v1n={" << v1n.x << "," << v1n.y << "}  v2n={" << v2n.x << "," << v2n.y << "}\n";
        std::cout << "  v1t={" << v1t.x << "," << v1t.y << "}  v2t={" << v2t.x << "," << v2t.y << "}\n";
        // --- END DEBUG ---



        if (e1->isUnmovable() && e2->isUnmovable())
        {
            /*Due entità inamovibili: nessuna fisica da calcolare*/
            return;
        }
        else if (e1->isUnmovable())
        {
            /*e1 resta ferma, e2 rimbalza — limite m1 → ∞*/
            float e          = dissipation;
            float new_v2n_scalar =  - e * v2n_scalar;
            new_v1 = v1;
            new_v2 = n * new_v2n_scalar + v2t;
        }
        else if (e2->isUnmovable())
        {
            /*e2 resta ferma, e1 rimbalza — limite m2 → ∞*/
            float e          = dissipation;
            float new_v1n_scalar = - e * v1n_scalar;
            new_v1 = n * new_v1n_scalar + v1t;
            new_v2 = v2;
        }
        else
        {
            /*Masse*/
            float m1 = e1->getMass();
            float m2 = e2->getMass();
            float M  = m1 + m2;
            float uM = m1*m2/M; //Massa ridotta

            /*L'energia cinetica totale diminuisce in base al fattore di dissipazione*/
            float current_KE = 0.5f*(m1*(v1*v1) + m2*(v2*v2));
            float eta        = 1.0f - dissipation;

            /*Coefficiente di restituzione ricavato da eta e dall'energia*/
            float vRel      = v1n_scalar - v2n_scalar;
            float e_squared = 1.0f - (2.0f * eta * current_KE) / (uM * vRel * vRel);
            e_squared       = std::max(0.0f, e_squared); // clamp per sicurezza numerica
            float e         = std::sqrt(e_squared);

            /*Nuove componenti normali delle velocità*/
            float new_v1n_scalar = ((m1 - e * m2) * v1n_scalar + m2 * (1.0f + e) * v2n_scalar) / M;
            float new_v2n_scalar = ((m2 - e * m1) * v2n_scalar + m1 * (1.0f + e) * v1n_scalar) / M;
            new_v1 = n * new_v1n_scalar + v1t;
            new_v2 = n * new_v2n_scalar + v2t;
        }

        std::cout   << "new_v1: {" << new_v1.x << ", " << new_v1.y << "}  "
                    << "new_v2: {" << new_v2.x << ", " << new_v2.y << "}\n";

        if (!e1->isUnmovable()) e1->addSpeedContribution(new_v1);
        if (!e2->isUnmovable()) e2->addSpeedContribution(new_v2);
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
        if(areAlreadyColliding(&h1->getParentEntity(), &h2->getParentEntity())) return false;

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
        const float nearestY = std::clamp(c.y, rect.top, rect.bottom);

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
                    m_collisions.push_back(CollisionCouple{activeEntity, active, checkEntity, check});
                    computeCollision(active, check);
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


    bool CollisionHandler::areAlreadyColliding(Entity* e1, Entity* e2)
    {
        for(CollisionCouple couple : m_collisions)
        {
            if(e1 == couple.first && e2 == couple.second ||
                e2 == couple.second && e1 == couple.first)
                    return true;
        }
        return false;
    }

    
    void CollisionHandler::removeInactiveCollisions()
    {
        auto it = m_collisions.begin();
        while (it != m_collisions.end())
        {
            if (!areOverlapping(it->first_hb, it->second_hb))
            {
                std::swap(*it, m_collisions.back());
                m_collisions.pop_back();
            }
            else
                it++;
        }
    }


    void CollisionHandler::printCollisionList()
    {
        for (const CollisionCouple& couple : m_collisions)
        {
            const Hitbox* h1 = couple.first_hb;
            const Hitbox* h2 = couple.second_hb;

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