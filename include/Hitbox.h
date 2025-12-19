#pragma once

#include "Klein.h"


namespace Klein
{
    class Hitbox
    {
        public:
            enum hitbox_type_t type;
            struct point_t center;
            float radius;
            int width;
            int height;
            int faction;
            float mass;
            int cellID;
            struct rectangle_t area;
            void* parent_entity;
            Hitbox(void* ENTITY, hitbox_type_t TYPE, point_t& CENTER, float RADIUS, int WIDTH, int HEIGHT)
            {
                //std::cout << "My Parent: " << ENTITY << std::endl;
                parent_entity = ENTITY;
                type = TYPE;
                center = CENTER;
                radius = RADIUS;
                width = WIDTH;
                height = HEIGHT;

                if(type == CIRCLE)
                {
                    area.top = center.y + radius;
                    area.bottom = center.y - radius;
                    area.right = center.x + radius;
                    area.left = center.x - radius;
                    width  = 2*radius;
                    height = width;
                }

                if(type == RECTANGLE)
                {
                    area.top    =   center.y + height/2;
                    area.bottom =   center.y - height/2;
                    area.right  =   center.x + width/2;
                    area.left   =   center.x - width/2;
                    radius = std::sqrt(width*width + height*height)/2;
                }
                mass = 0;
                cellID = 0;
            }
            
            ~Hitbox() = default;

            point_t getPosition();

            void setWidth(int w);
            void setHeight(int h);
            void setRadius(float r);

            int getTop();
            int getBottom();
            int getLeft();
            int getRight();
            float getRadius();

            void* getParentEntity();
    };
}