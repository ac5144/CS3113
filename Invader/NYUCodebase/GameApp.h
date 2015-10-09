#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "Matrix.h"
#include "ShaderProgram.h"
#include "Entity.h"
#include "Projectile.h"
#include <vector>
#include <string>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

enum GameState { MAIN_MENU, GAME_LEVEL, GAME_OVER };

class GameApp
{
public:
	GameApp();
	~GameApp();

	void Setup();
	void ProcessEvents();
	void Update();
	void Render();
	bool UpdateAndRender();

	void RenderMainMenu();
	void RenderGame();
	void RenderGameOver();

	GLuint LoadTexture(const char* image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing);

private:
	SDL_Window* displayWindow;
	SDL_Event event;
	bool done;
	ShaderProgram* program;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	GameState state;

	Entity* player;
	std::vector<Projectile*> bullets;
	std::vector<Entity*> aliens;

	GLuint fontID;
	GLuint playerID;
};

