#include "Hitbox.h"
#include <cmath>

namespace Klein
{
    /*Costruttori*/
    
    Hitbox::Hitbox(Entity& parent, const circle_t& shape)
        : m_parent(parent)
        , m_type(HitboxType::Circle)
        , m_center(shape.center)
        , m_radius(static_cast<float>(shape.radius))
    {
        computeAABB();
    }

    Hitbox::Hitbox(Entity& parent, const rectangle_t& shape)
        : m_parent(parent)
        , m_type(HitboxType::Rectangle)
        , m_center(shape.origin())
        , m_width(shape.width())
        , m_height(shape.height())
    {
        computeAABB();
    }


    void Hitbox::computeAABB()
    {
        if (m_type == HitboxType::Circle)
        {
            m_area.top    = static_cast<int>(m_center.y + m_radius);
            m_area.bottom = static_cast<int>(m_center.y - m_radius);
            m_area.right  = static_cast<int>(m_center.x + m_radius);
            m_area.left   = static_cast<int>(m_center.x - m_radius);
            m_width       = static_cast<int>(2 * m_radius);
            m_height      = m_width;
        }
        else
        {
            m_area.top    = m_center.y + m_height / 2;
            m_area.bottom = m_center.y - m_height / 2;
            m_area.right  = m_center.x + m_width  / 2;
            m_area.left   = m_center.x - m_width  / 2;
            m_radius      = std::sqrt(static_cast<float>(m_width*m_width + m_height*m_height)) / 2.f;
        }
        assert(abs(m_area.top - m_area.bottom) <= MAX_PARTICLE_SIZE);
        assert(abs(m_area.right - m_area.left) <= MAX_PARTICLE_SIZE);
    }


    /*Getters*/

    HitboxType  Hitbox::getType()         const { return m_type;             }
    point_t     Hitbox::getCenter()       const { return m_center;           }
    float       Hitbox::getRadius()       const { return m_radius;           }
    int         Hitbox::getWidth()        const { return m_width;            }
    int         Hitbox::getHeight()       const { return m_height;           }
    int         Hitbox::getFaction()      const { return m_faction;          }
    float       Hitbox::getMass()         const { return m_mass;             }
    int         Hitbox::getCellID()       const { return m_cellID;           }
    rectangle_t Hitbox::getArea()         const { return m_area;             }
    Entity&     Hitbox::getParentEntity() const { return m_parent;           }


    /*Setters*/

    void Hitbox::setFaction(int faction) { m_faction = faction; }
    void Hitbox::setMass(float mass)     { m_mass    = mass;    }
    void Hitbox::setCellID(int id)       { m_cellID  = id;      }

    void Hitbox::setRadius(float r)
    {
        KLEIN_ASSERT(m_type == HitboxType::Circle);
        m_radius = r;
        computeAABB();
    }

    void Hitbox::setWidth(int w)
    {
        KLEIN_ASSERT(m_type == HitboxType::Rectangle);
        m_width = w;
        computeAABB();
    }

    void Hitbox::setHeight(int h)
    {
        KLEIN_ASSERT(m_type == HitboxType::Rectangle);
        m_height = h;
        computeAABB();
    }

} // namespace Klein