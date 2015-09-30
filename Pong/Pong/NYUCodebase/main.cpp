#include "PongApp.h"
/*
	SUPER AWESOME PONG V1.0
	
	CONTROLS:
	W: Left paddle moves up
	S: Left paddle moves down
	UP: Right paddle moves up
	DOWN: Right paddle moves down

	Winner of the round gets the triangle trophy of ultimate bragging rights
*/
GLuint LoadTexture(const char* image_path)
{
	SDL_Surface* surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}


int main(int argc, char *argv[])
{
	PongApp pong;
	while (!pong.UpdateAndRender()) {}
	return 0;
}
