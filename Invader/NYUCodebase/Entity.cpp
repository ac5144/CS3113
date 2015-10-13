#include "Entity.h"

Entity::Entity(float x, float y, float h, float w, ENTITY_TYPE type) : original_x(x), original_y(y), height(h), width(w), alive(true), type(type)
{
}

float Entity::getX() const { return original_x; }
float Entity::getY() const { return original_y; }

float Entity::getHeight() const { return height; }
float Entity::getWidth() const { return width; }

void Entity::move_x(float elapsed)
{
	original_x += elapsed;
}

void Entity::move_y(float elapsed)
{
	original_y += elapsed;
}

bool Entity::isAlive() const { return alive; }
ENTITY_TYPE Entity::getType() const { return type; };
void Entity::dies() { alive = false; }