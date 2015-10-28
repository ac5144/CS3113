#pragma once
#include "SheetSprite.h"
#include <vector>
class Entity
{
public:
	Entity();
	void Render(ShaderProgram* program, Matrix& modelMatrix);
	void Update(float elapsed);
	bool collidesWith(Entity *entity);
	void resetFlags();

	float x;
	float y;

	float height;
	float width;

	float velocity_x;
	float velocity_y;

	float acceleration_x;
	float acceleration_y;

	float friction;

	SheetSprite* sprite;

	bool isStatic;

	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;
};

