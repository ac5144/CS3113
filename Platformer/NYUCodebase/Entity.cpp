#include "Entity.h"

Entity::Entity()
{
}

void Entity::Update(float elapsed)
{
	resetFlags();
	x += velocity_x * elapsed;
	y += velocity_y * elapsed;
	velocity_x += acceleration_x * elapsed;
	if (velocity_x > 0.0)
		velocity_x += -friction * elapsed;
	else if (velocity_x < 0.0)
		velocity_x += friction * elapsed;
	velocity_y += acceleration_y * elapsed;
	/*
	if (acceleration_x > 0.0)
	{
		acceleration_x -= friction * elapsed;
		if (acceleration_x < 0.5)
			acceleration_x = 0.0;
	}
	else if (acceleration_x < 0.0)
	{
		acceleration_x += friction * elapsed;
		if (acceleration_x > -0.5)
			acceleration_x = 0.0;
	}*/
}

void Entity::Render(ShaderProgram* program, Matrix& modelMatrix)
{
	sprite->Draw(8, 8, 4, program, modelMatrix, x, y);
}

bool Entity::collidesWith(Entity *entity)
{
	float left = x - (width / 2.0);
	float right = x + (width / 2.0);
	float top = y + (height / 2.0);
	float bot = y - (height / 2.0);
	float otherLeft = entity->x - (entity->width / 2.0);
	float otherRight = entity->x + (entity->width / 2.0);
	float otherTop = entity->y + (entity->height / 2.0);
	float otherBot = entity->y - (entity->height / 2.0);
	if (bot < otherTop && 
		fabs(left - otherLeft) < entity->width && 
		fabs(right - otherRight) < entity->width)
	{
		float penetration = fabs(bot - otherTop);
		y += penetration + 0.001;
		collidedBottom = true;
		return true;
	}
	else if (top > otherBot && 
		fabs(left - otherLeft) < entity->width && 
		fabs(right - otherRight) < entity->width && 
		bot < otherTop)
	{
		float penetration = fabs(top - otherBot);
		y -= penetration + 0.001;
		collidedBottom = true;
		return true;
	}
	if (left < otherRight && 
		fabs(top - otherTop) < entity->height && 
		fabs(bot - otherBot) < entity->height && 
		right > otherLeft)
	{
		float penetration = fabs(left - otherRight);
		x += penetration + 0.001;
		collidedLeft = true;
		return true;
	}
	else if (right > otherLeft && 
		fabs(top - otherTop) < entity->height && 
		fabs(bot - otherBot) < entity->height && 
		left < otherRight)
	{
		float penetration = fabs(right - otherLeft);
		x -= penetration + 0.001;
		collidedRight = true;
		return true;
	}
	return false;
}

void Entity::resetFlags()
{
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
}