#pragma once

#include "Klein.h"

namespace Klein
{
    class Hitbox
    {
        public:
            enum HITBOX_TYPE type;
            struct Point center;
            float radius;
            int width;
            int height;
            int faction;
            float mass;
            int cell_id;
            struct Rectangle area;
            void* parent_entity;
            Hitbox(void* ENTITY, HITBOX_TYPE TYPE, Point* CENTER, float RADIUS, int WIDTH, int HEIGHT)
            {
                //std::cout << "My Parent: " << ENTITY << std::endl;
                parent_entity = ENTITY;
                type = TYPE;
                center = *CENTER;
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
                    area.top = center.y + height/2;
                    area.bottom = center.y - height/2;
                    area.right = center.x + width/2;
                    area.left = center.x - width/2;
                    radius = std::sqrtf(width*width + height*height)/2;
                }
                mass = 0;
                cell_id = 0;
            }
            
            ~Hitbox() = default;

            Point GetPosition();

            void SetWidth(int w);
            void SetHeight(int h);
            void SetRadius(float r);

            int GetTop();
            int GetBottom();
            int GetLeft();
            int GetRight();
            float GetRadius();

            void* GetParentEntity();
    };
}