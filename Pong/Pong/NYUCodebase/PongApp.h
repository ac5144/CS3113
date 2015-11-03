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
#include <SDL_mixer.h>

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
	void ProcessEvents(float elapsed);
	void Update(float elapsed);
	void Render();
	bool UpdateAndRender();
private:
	ShaderProgram* program;

	SDL_Window* displayWindow;

	SDL_Event event;

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	float lastFrameTicks;

	Paddle* leftPaddle;
	Paddle* rightPaddle;

	Ball* gameBall;

	bool done;
	int winner;

	Mix_Chunk *bumpSound;
	Mix_Music *bg;
};

