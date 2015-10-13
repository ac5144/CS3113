#pragma once
#include "Entity.h"
#define VELOCITY 1.0
#define BULLET_HEIGHT 0.5f
#define BULLET_WIDTH 0.5f

class Bullet
{
public:
	Bullet(float x, float y, Entity* source);

	void move(float elapsed);
	float getXposition() const;
	float getYposition() const;
	float getTTL() const;
	Entity* getSource();

private:
	float position_x;
	float position_y;

	float timeToLive;

	Entity* source;
};

