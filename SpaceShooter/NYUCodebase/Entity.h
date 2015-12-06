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
#include <vector>

class Entity
{
public:
	Entity(float x, float y, float h, float w, GLuint id);
	~Entity();

	//	RENDER
	void Render(ShaderProgram *program, Matrix& modelMatrix);

	//	VARIABLES
	float x;
	float y;

	float height;
	float width;

	GLuint textureID;
};

