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
#include "Entity.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
enum GameState {MAIN_MENU, LEVEL_1, LEVEL_2, LEVEL_3};

class GameApp
{
public:
	GameApp();
	~GameApp();

	// SETUP
	void Setup();

	//	UPDATE
	void ProcessEvents(float elapsed);
	void Update(float elapsed);
	bool UpdateAndRender();
	
	//	RENDER
	void Render();
	void RenderMainMenu();
	void RenderLevel1();
	void RenderLevel2();
	void RenderLevel3();

	//	GAME TOOLS

	//	UTILITY
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTexture(const char* image_path);
	void changeMusic(Mix_Music *m);

	//--------------------------------------------------------------------------------//

	//	VARIABLES
	SDL_Window* displayWindow;
	Matrix modelMatrix;
	Matrix projectionMatrix;
	Matrix viewMatrix;
	ShaderProgram *program;
	SDL_Event event;
	bool done;

	// GAME VARIABLES
	Entity* player;

	// TEXTURE ID
	int fontID;
	int playerID;

	// UTILITY
	float lastFrameTicks;
	GameState state;
	bool stateSwitched;

	//MUSIC
	Mix_Music *menuMusic;
	Mix_Music *gameMusic;
};

