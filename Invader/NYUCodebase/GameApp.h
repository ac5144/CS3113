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
#include "Bullet.h"
#include <vector>
#include <string>
#include <ctime>

#define DEF_NUM_ALIENS 15

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
	void ProcessEvents(float elapsed);
	void Update(float elapsed);
	void Render();
	bool UpdateAndRender();

	void RenderMainMenu();
	void RenderGame();
	void RenderGameOver();

	GLuint LoadTexture(const char* image_path);
	void DrawText(int fontTexture, std::string text, float size, float spacing);
	void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY, int textureID, float x, float y);
	
	bool playerBulletLive();
	void cleanBullets();
	void setAliens(std::vector<Entity*>& v, int n = DEF_NUM_ALIENS);
	void aliensAttack();
	void aliensMove(float elapsed);
	bool collision(Bullet* b, Entity* e);
	void checkCollisions();
	bool aliensDead();
	void checkWinStatus();
	void resetGame();
private:
	SDL_Window* displayWindow;
	SDL_Event event;
	bool done;
	ShaderProgram* program;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	GameState state;
	float lastFrameTicks;

	Entity* player;
	std::vector<Entity*> aliens;
	std::vector<Bullet*> bullets;

	GLint fontID;
	GLuint playerID;
	GLuint bulletID;
	GLuint alienID;

	float attackInterval;
	float moveInterval;

	bool win;
};

