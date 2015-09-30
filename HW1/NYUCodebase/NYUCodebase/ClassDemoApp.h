#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Matrix.h"
#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class ClassDemoApp
{
public:
	ClassDemoApp();
	~ClassDemoApp();
	void Setup();
	void ProcessInput();
	void Render();
	void Update();
	void UpdateAndRender();
	bool isDone();

private:
	ShaderProgram* program;

	SDL_Window* displayWindow;

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	float angle;
	float lastFrameTicks;
	float xDir;
	float yDir;
	float positionX;
	float positionY;
	bool done;
};
