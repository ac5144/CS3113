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
#include "Bullet.h"

enum Type { PLAYER, ASTEROID };

class Entity
{
public:
	Entity(float x, float y, float h, float w, GLuint id);
	~Entity();

	// UPDATE
	void Update(float elapsed);

	// MOVEMENT
	void move_x(float elapsed);
	void move_y(float elapsed);

	//	RENDER
	void Render(ShaderProgram *program, Matrix& modelMatrix);

	// GAME TOOLS
	void expires();
	void shoot(std::vector<Bullet*>& v, GLuint id);

	// UTILITY
	float lerp(float v0, float v1, float t);

	//----------------------------------------------------------------------//

	//	VARIABLES

	// POSITION
	float x;
	float y;

	// MOVEMENT
	float vel_x;
	float vel_y;

	float acc_x;
	float acc_y;

	// DIMENSIONS
	float height;
	float width;


	float shootCooldown;

	// STATUS
	Type type;
	bool alive;

	GLuint textureID;
};

