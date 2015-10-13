#include "Bullet.h"

Bullet::Bullet(float x, float y, Entity* source) : position_x(x), position_y(y), source(source), timeToLive(1.5)
{
	if (source->getType() == PLAYER)
		position_y += 1.0;
	else
		position_y -= 1.0;
}

void Bullet::move(float elapsed)
{
	if (source->getType() == PLAYER)
		position_y += elapsed * 10.0;
	else
		position_y -= elapsed * 10.0;

	timeToLive -= elapsed;
}

float Bullet::getXposition() const { return position_x; }
float Bullet::getYposition() const { return position_y; }

float Bullet::getTTL() const { return timeToLive; }
Entity* Bullet::getSource() { return source; }