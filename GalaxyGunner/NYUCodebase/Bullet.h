#pragma once

#pragma once
#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#define BULLET_HEIGHT 0.5
#define BULLET_WIDTH 0.5
class Bullet
{
public:
	Bullet(GLuint id);
	~Bullet();

	// UPDATE
	void Update(float elapsed);

	// MOVEMENT
	void move_y(float elapsed);

	// RENDER
	void Render(ShaderProgram *program, Matrix& modelMatrix);

	//----------------------------------------------------------------//
	// VARIABLES
	float x;
	float y;

	// MOVEMENT
	float vel_x;
	float vel_y;

	bool live;

	GLuint textureID;
};

