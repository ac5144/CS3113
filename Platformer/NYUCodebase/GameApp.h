#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Entity.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class GameApp
{
public:
	GameApp();
	~GameApp();

	GLuint LoadTexture(const char* image_path);

	void buildLevel();
	void renderTiles();
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);

	void scroll();
	void playerCollisions();

	void Setup();
	void ProcessEvents(float elapsed);
	void Update(float elapsed);
	void Render();
	bool UpdateAndRender();

private:
	SDL_Window* displayWindow;
	SDL_Event event;
	bool done;
	ShaderProgram* program;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	float lastFrameTicks;

	int mapWidth;
	int mapHeight;
	unsigned char** levelData;

	Entity* player;
	std::vector<Entity*> blocks;
	std::vector<int> solids;

	unsigned int tileID;
	unsigned int playerID;
};

