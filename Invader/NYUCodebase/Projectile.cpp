#include "Projectile.h"


Projectile::Projectile(float x, float y, float v) : position_x(x), position_y(y), velocity_y(v)
{
}


Projectile::~Projectile()
{
}

void Projectile::move_y(float y) { position_y += velocity_y; }

float Projectile::getVelocityY() const { return velocity_y; }
