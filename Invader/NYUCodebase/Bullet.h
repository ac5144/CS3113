#pragma once
#include "Entity.h"
#define BULLET_HEIGHT 0.5f
#define BULLET_WIDTH 0.5f

class Bullet
{
public:
	Bullet(float x, float y, Entity* source);

	void move_y(float y);
	float getXposition() const;
	float getYposition() const;
	float getTTL() const;
	Entity* getSource();

	void draw(GLuint textureID, ShaderProgram* program, Matrix& modelMatrix);

private:
	float position_x;
	float position_y;
	Entity* source;
	float ttl;
};

