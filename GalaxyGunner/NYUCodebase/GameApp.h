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
enum GameState {MAIN_MENU, CUTSCENE_1, LEVEL_1, CUTSCENE_2, CUTSCENE_3, LEVEL_2, CUTSCENE_4, CUTSCENE_5, LEVEL_3, CUTSCENE_6, GAME_OVER, GAME_WIN};

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
	void UpdateMainMenu();
	void UpdateEnemies(float elapsed);
	void UpdateGame(float elapsed);
	void UpdateIntroLevel(float elapsed);
	void UpdateLevelEnd(float elapsed);
	bool UpdateAndRender();
	
	//	RENDER
	void Render();
	void RenderMainMenu();
	void RenderGameLevel();
	void RenderLevelIntro();
	void RenderLevelEnd();
	void RenderGameOver();
	void RenderGameWin();
	void RenderEnemies();
	void RenderBullets();

	// LOAD VARIABLES
	void LoadMusicAndSounds();
	void LoadTextureIDs();
	void LoadCutsceneVar();

	//	GAME TOOLS
	bool CanBePlaced(Entity* e, std::vector<Entity*> v);
	void CleanBullets();
	void CleanEnemies();
	bool LevelComplete();
	void CheckGameOver();
	bool at_x(Entity* e, float goal);
	void ClearObjects();

	// OBJECT CREATION
	void GeneratePlayer();
	void GenerateEnemies();
	Entity* CreateAsteroid();
	Entity* CreateGrunt();

	// COLLISION TOOLS
	bool BulletCollides(Entity* e, Bullet* b);
	void BulletEnemyCollision();
	bool Collides(Entity* e1, Entity* e2);
	void PlayerCollision();

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
	std::vector<Entity*> enemies;
	std::vector<Bullet*> bullets;

	// TEXTURE ID
	int fontID;
	int playerID;
	int asteroidID;
	int laserID;
	int gruntID;
	int astroID;

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

	float level_x2;
	float level_y2;

	float level_cd;
	bool cutscene_done;
	bool up;
	bool down;
};

