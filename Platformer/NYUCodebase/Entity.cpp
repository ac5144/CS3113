#include "Entity.h"

Entity::Entity()
{
}

void Entity::Update(float elapsed)
{
	y += velocity_y * elapsed;
	velocity_y += acceleration_y * elapsed;
}

void Entity::Render(ShaderProgram* program, Matrix& modelMatrix)
{
	sprite->Draw(8, 8, 4, program, modelMatrix, x, y);
}

bool Entity::collidesWith(Entity *entity)
{
	float left = x - width / 2.0;
	float right = x + width / 2.0;
	float top = y + height / 2.0;
	float bot = y - height / 2.0;
	float otherLeft = entity->x - entity->width / 2.0;
	float otherRight = entity->x + entity->width / 2.0;
	float otherTop = entity->y + entity->height / 2.0;
	float otherBot = entity->y - entity->height / 2.0;
	if (bot < otherTop)
	{
		float penetration = fabs((y = entity->y) - height / 2 - entity->height / 2);
		y += penetration + 0.001;
		collidedBottom = true;
		return true;
	}
	return false;
}