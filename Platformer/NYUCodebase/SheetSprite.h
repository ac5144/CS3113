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
class SheetSprite
{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float width, float height, float size);

	void Draw(int index, int spriteCountX, int spriteCountY, ShaderProgram* program, Matrix& modelMatrix);
	void DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY, ShaderProgram* program, Matrix& modelMatrix, float x, float y);
private:
	float size;
	unsigned int textureID;
	float width;
	float height;
};

