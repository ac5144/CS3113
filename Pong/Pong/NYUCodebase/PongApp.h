#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Matrix.h"
#include "ShaderProgram.h"
#include "Paddle.h"
#include "Ball.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class PongApp
{
public:
	PongApp();
	~PongApp();

	void Setup();
	void ProcessEvents();
	void Update();
	void Render();
	bool UpdateAndRender();
private:
	ShaderProgram* program;

	SDL_Window* displayWindow;

	SDL_Event event;

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	Paddle* leftPaddle;
	Paddle* rightPaddle;

	Ball* gameBall;

	bool done;
	int winner;
};

