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
#include "Bullet.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
enum GameState {MAIN_MENU, LEVEL_1, CUTSCENE_1, LEVEL_2, LEVEL_3, GAME_OVER, GAME_WIN};

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
	void UpdateEnemies(float elapsed);
	void UpdateLevel1(float elapsed);
	void UpdateCutscene1(float elapsed);
	bool UpdateAndRender();
	
	//	RENDER
	void Render();
	void RenderMainMenu();
	void RenderLevel1();
	void RenderLevel2();
	void RenderLevel3();
	void RenderCutScene1();
	void RenderLevel1Complete();
	void RenderGameOver();

	//	GAME TOOLS
	void generatePlayer();
	void generateEnemies();
	Entity* createAsteroid();
	void renderEnemies();
	void renderBullets();
	bool canBePlaced(Entity* e, std::vector<Entity*> v);
	void cleanBullets();
	void cleanEnemies();
	bool levelComplete();
	void setUpCutscene();
	void LoadCutsceneVar();
	void checkGameOver();

	// COLLISION TOOLS
	bool bulletCollides(Entity* e, Bullet* b);
	void bulletToEnemy();
	bool collides(Entity* e1, Entity* e2);
	void checkPlayerCollision();

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

	// ENEMIES & OBJECTS
	std::vector<Entity*> level1Enemies;
	std::vector<Bullet*> bullets;

	// TEXTURE ID
	int fontID;
	int playerID;
	int asteroidID;
	int laserID;

	// UTILITY
	float lastFrameTicks;
	GameState state;
	bool stateSwitched;

	// MUSIC & SOUNDS
	Mix_Music *menuMusic;
	Mix_Music *gameMusic;

	Mix_Chunk *explosionSound;
	Mix_Chunk *laserSound;

	// CUTSCENE VARIABLES
	float level_x;
	float level_y;

	float level_cd;
	bool cutscene_done;
	bool up;
	bool down;
};

