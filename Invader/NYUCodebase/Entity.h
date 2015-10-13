#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>

enum ENTITY_TYPE {PLAYER, ALIEN};

class Bullet;

class Entity
{
public:
	Entity(float x, float y, float h, float w, ENTITY_TYPE type);


	float getXposition() const;
	float getYposition() const;
	bool isAlive() const;
	ENTITY_TYPE getType() const;
	float getHeight() const;
	float getWidth() const;

	void move_x(float x);
	void move_y(float y);
	void shoot(std::vector<Bullet*>& v);
	void dies();
	void draw(GLuint textureID, ShaderProgram* program, Matrix& modelMatrix);

private:
	float position_x;
	float position_y;

	ENTITY_TYPE type;

	bool alive;

	float height;
	float width;
};

