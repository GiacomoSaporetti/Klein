#include "Hitbox.h"
#include "Entity.h"
#include <cmath>

namespace Klein
{
    /*Costruttori*/
    
    Hitbox::Hitbox(Entity& parent, const circle_t& shape)
        : m_parent(parent)
        , m_type(HitboxType::Circle)
        , m_offset(shape.center)
        , m_radius(static_cast<float>(shape.radius))
    {
        parent.addHitbox(this);
        updateAABB();
    }

    Hitbox::Hitbox(Entity& parent, const rectangle_t& shape)
    : m_parent(parent)
    , m_type(HitboxType::Rectangle)
    , m_width(shape.width())
    , m_height(shape.height())
    {
        const point_t parentPos = parent.getPosition();
        const point_t shapeCenter = shape.center();   //centro geometrico, non origin()
        m_offset = { shapeCenter.x, shapeCenter.y};
        parent.addHitbox(this);
        updateAABB();
    }


    void Hitbox::updateAABB()
    {
        if (m_type == HitboxType::Circle)
        {
            m_area.top    = (m_offset.y - m_radius);
            m_area.bottom = (m_offset.y + m_radius);
            m_area.right  = (m_offset.x + m_radius);
            m_area.left   = (m_offset.x - m_radius);
            m_width       = (2 * m_radius);
            m_height      = m_width;
        }
        else
        {
            m_area.top    = m_offset.y - m_height / 2;
            m_area.bottom = m_offset.y + m_height / 2;
            m_area.right  = m_offset.x + m_width  / 2;
            m_area.left   = m_offset.x - m_width  / 2;
            m_radius      = std::sqrt(m_width*m_width + m_height*m_height) / 2.f;
        }
    }

    /*Getters*/

    HitboxType  Hitbox::getType()         const { return m_type;             }
    float       Hitbox::getRadius()       const { return m_radius;           }
    int         Hitbox::getWidth()        const { return m_width;            }
    int         Hitbox::getHeight()       const { return m_height;           }
    int         Hitbox::getFaction()      const { return m_faction;          }
    float       Hitbox::getMass()         const { return m_mass;             }
    int         Hitbox::getCellID()       const { return m_cellID;           }
    Entity&     Hitbox::getParentEntity() const { return m_parent;           }

    rectangle_t Hitbox::getBoundingBox() const
    {
        const point_t pos = m_parent.getPosition();
        rectangle_t area;
        area.top    = m_area.top    + pos.y;
        area.bottom = m_area.bottom + pos.y;
        area.right  = m_area.right  + pos.x;
        area.left   = m_area.left   + pos.x;
        return area;
    }

    point_t Hitbox::getCenter() const
    {
        const point_t pos = m_parent.getPosition();
        return { pos.x + m_offset.x, pos.y + m_offset.y };
    }


    /*Setters*/

    void Hitbox::setFaction(int faction) { m_faction = faction; }
    void Hitbox::setMass(float mass)     { m_mass    = mass;    }
    void Hitbox::setCellID(int id)       { m_cellID  = id;      }

    void Hitbox::setRadius(float r)
    {
        KLEIN_ASSERT(m_type == HitboxType::Circle);
        m_radius = r;
        updateAABB();
    }

    void Hitbox::setWidth(int w)
    {
        KLEIN_ASSERT(m_type == HitboxType::Rectangle);
        m_width = w;
        updateAABB();
    }

    void Hitbox::setHeight(int h)
    {
        KLEIN_ASSERT(m_type == HitboxType::Rectangle);
        m_height = h;
        updateAABB();
    }

} // namespace Klein