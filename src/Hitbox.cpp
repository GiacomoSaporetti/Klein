#include "Hitbox.h"

using namespace Klein;
using std::sqrt;

point_t Hitbox::getPosition(){return center;}

void Hitbox::setWidth(int w)
{
    width = w;
    area.right = center.x + w/2;
    area.left = center.x - w/2;
    radius = sqrt(width*width + height*height)/2;
}

void Hitbox::setHeight(int h)
{
    height = h;
    area.top = center.y + h/2;
    area.bottom = center.y - h/2;
    radius = sqrt(width*width + height*height)/2;
}

void Hitbox::setRadius(float r)
{
    radius = r;
    area.right = center.x + r;
    area.left = center.x - r;
    area.top = center.y + r;
    area.bottom = center.y - r;
}

int Hitbox::getTop(){return area.top;}

int Hitbox::getBottom(){return area.bottom;}

int Hitbox::getLeft(){return area.left;}

int Hitbox::getRight(){return area.right;}

void* Hitbox::getParentEntity(){return parent_entity;}

float Hitbox::getRadius(){return radius;}