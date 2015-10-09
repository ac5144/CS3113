#include "GameApp.h"

int main(int argc, char *argv[])
{
	GameApp game;
	while (!game.UpdateAndRender()) {}

	return 0;
}
