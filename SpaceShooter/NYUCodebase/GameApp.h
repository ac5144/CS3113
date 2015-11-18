#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include <vector>
#include <string>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
enum GameState {MAIN_MENU, LEVEL_1};

class GameApp
{
public:
	GameApp();
	~GameApp();

	void Setup();
	void ProcessEvents(float elapsed);
	void Update(float elapsed);
	void Render();
	bool UpdateAndRender();
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTexture(const char* image_path);
	void RenderMainMenu();

private:
	SDL_Window* displayWindow;
	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;
	SDL_Event event;
	bool done;
	int fontID;
	float lastFrameTicks;
	GameState state;
	Mix_Music *menuMusic;
};

