#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SheetSprite.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class Entity
{
public:
	Entity(unsigned int textureID);
	~Entity();

	void Update(float elapsed);
	void Render(ShaderProgram *program, Matrix& modelMatrix);
	bool collidesWith(Entity *entity);

	float getX() const;
	float getY() const;

	float getHeight() const;
	float getWidth() const;
private:
	SheetSprite sprite;

	float x;
	float y;

	float width;
	float height;

	float velocity_x;
	float velocity_y;

	float acceleration_x;
	float acceleration_y;

	bool isStatic;

	bool collidedTop;
	bool collidedBot;
	bool collidedLeft;
	bool collidedRight;
};

