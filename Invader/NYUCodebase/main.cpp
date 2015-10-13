#include "GameApp.h"
/*
	SPACE INVADERS
	LEFT		Move left
	RIGHT		Move Right
	SPACEBAR	Shoot
*/


int main(int argc, char *argv[])
{
	GameApp game;
	while (!game.UpdateAndRender()) {}

	return 0;
}
