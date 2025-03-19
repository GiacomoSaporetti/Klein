#include "Hitbox.h"

using namespace Klein;

Point Hitbox::GetPosition()
{return center;}

void Hitbox::SetWidth(int w)
{
    width = w;
    area.right = center.x + w/2;
    area.left = center.x - w/2;
    radius = std::sqrtf(width*width + height*height)/2;
}

void Hitbox::SetHeight(int h)
{
    height = h;
    area.top = center.y + h/2;
    area.bottom = center.y - h/2;
    radius = std::sqrtf(width*width + height*height)/2;
}

void Hitbox::SetRadius(float r)
{
    radius = r;
    area.right = center.x + r;
    area.left = center.x - r;
    area.top = center.y + r;
    area.bottom = center.y - r;
}

int Hitbox::GetTop()
{return area.top;}

int Hitbox::GetBottom()
{return area.bottom;}

int Hitbox::GetLeft()
{return area.left;}

int Hitbox::GetRight()
{return area.right;}

void* Hitbox::GetParentEntity()
{return parent_entity;}

float Hitbox::GetRadius()
{return radius;}