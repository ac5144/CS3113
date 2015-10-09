#include "Entity.h"

Entity::Entity(float x, float y) : position_x(x), position_y(y), alive(true)
{
}

void Entity::move_x(float x) { position_x += x; }
void Entity::move_y(float y) { position_y += y; }

float Entity::getXposition() const { return position_x; }
float Entity::getYposition() const { return position_y; }

bool Entity::isAlive() const { return alive; }