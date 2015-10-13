#include "GameApp.h"

GameApp::GameApp() : done(false), lastFrameTicks(0.0), win(false)
{
	player = new Entity(0.0, -7.5, 1.0, 2.0, PLAYER);
	state = MAIN_MENU;
	shoot_interval = 2.0;
	move_interval = 5.0;
	Setup();
}

GameApp::~GameApp()
{
	SDL_Quit();
}

GLuint GameApp::LoadTexture(const char* image_path)
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

void GameApp::DrawText(int fontTexture, std::string text, float size, float spacing)
{
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	
	for (int i = 0; i < text.size(); i++) 
	{ 
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), { 
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size, 
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size, 
			((size + spacing) * i) + (0.5f * size), 0.5f * size, 
			((size + spacing) * i) + (-0.5f * size), -0.5f * size, 
		});
		texCoordData.insert(texCoordData.end(), { 
			texture_x, texture_y, 
			texture_x, texture_y + texture_size, 
			texture_x + texture_size, 
			texture_y, texture_x + texture_size, 
			texture_y + texture_size, texture_x + texture_size, 
			texture_y, texture_x, 
			texture_y + texture_size, 
		}); 
	}
	
	glUseProgram(program->programID);
	
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void GameApp::DrawSpriteSheetSprite(int index, int spriteCountX, int spriteCountY, float x = 0.0, float y = 0.0)
{
	float u = (float)(((int)index) % spriteCountX) / (float) spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float) spriteCountY;
	float spriteWidth = 1.0 / (float) spriteCountX;
	float spriteHeight = 1.0 / (float) spriteCountY;

	GLfloat texCoords[] = {
		u, v + spriteHeight,
		u + spriteWidth, v,
		u, v,
		u + spriteWidth, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight,
	};

	float vertices[] = { -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	modelMatrix.identity();
	modelMatrix.Translate(x, y, 0.0);
	program->setModelMatrix(modelMatrix);

	glBindTexture(GL_TEXTURE_2D, alienID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

bool GameApp::playerFired()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getSource()->getType() == PLAYER)
			return true;
	}
	return false;
}

bool GameApp::collision(Bullet* b, Entity* e)
{
	float bulletTop = b->getYposition() + BULLET_HEIGHT / 2.0;
	float bulletBot = b->getYposition() - BULLET_HEIGHT / 2.0;
	float bulletLeft = b->getXposition() - BULLET_WIDTH / 2.0;
	float bulletRight = b->getXposition() + BULLET_WIDTH / 2.0;

	float entityTop = e->getY() + e->getHeight() / 2.0;
	float entityBot = e->getY() - e->getHeight() / 2.0;
	float entityLeft = e->getX() - e->getWidth() / 2.0;
	float entityRight = e->getX() + e->getWidth() / 2.0;

	return !((bulletBot > entityTop) ||
		(bulletTop < entityBot) ||
		(bulletLeft > entityRight) ||
		(bulletRight < entityLeft));
}

void GameApp::checkCollisions()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getSource()->getType() == ALIEN)
		{
			if (player->isAlive())
			{
				if (collision(bullets[i], player))
				{
					player->dies();
					bullets.erase(bullets.begin() + i);
					state = GAME_OVER;
					--i;
					break;
				}
			}
				
		}
		else
		{
			for (int j = 0; j < aliens.size(); j++)
			{
				if (aliens[j]->isAlive())
				{
					if (collision(bullets[i], aliens[j]))
					{
						aliens[j]->dies();
						bullets.erase(bullets.begin() + i);
						--i;
						break;
					}
				}
			}
		}
	}
}

void GameApp::Setup()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);

	//program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	projectionMatrix.setOrthoProjection(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f);

	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	glUseProgram(program->programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	fontID = LoadTexture("font1.png");
	playerID = LoadTexture("ship.png");
	bulletID = LoadTexture("bullet.png");
	alienID = LoadTexture("invaders.png");

	float x = -8.0;
	float y = 6.0;

	for (int i = 0; i < 15; i++)
	{
		aliens.push_back(new Entity (x, y, 0.5, 0.5, ALIEN));
		x += 4.0;
		if (i != 0 && (i + 1) % 5 == 0)
		{
			x = -8.0;
			y -= 3.0;
		}
	}
}

void GameApp::ProcessEvents(float elapsed)
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}

		const Uint8* keys = SDL_GetKeyboardState(NULL);

		switch (state)
		{
		case MAIN_MENU:
			if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_RETURN2])
				state = GAME_LEVEL;
			break;
		case GAME_LEVEL:
			if (keys[SDL_SCANCODE_LEFT])
			{
				if (player->getX() > -14.0)
					player->move_x(-1.0f * elapsed * 100);
			}
			else if (keys[SDL_SCANCODE_RIGHT])
			{
				if (player->getX() < 14.0)
					player->move_x(1.0f * elapsed * 100);
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
				{
					if (!playerFired())
					{
						//Shoot Bullet
						bullets.push_back(new Bullet(player->getX(), player->getY(), player));
					}
				}
			}
			break;
		}
	}
}

void GameApp::Update(float elapsed)
{
	ProcessEvents(elapsed);

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->getTTL() <= 0.0)
		{
			bullets.erase(bullets.begin() + i);
			--i;
		}
		else
			bullets[i]->move(elapsed);
	}
	
	shoot_interval -= elapsed;
	move_interval -= elapsed;
	if (shoot_interval <= 0.0)
	{
		srand(time(NULL));
		int shoot = rand() % 15;
		while (!aliens[shoot]->isAlive())
		{
			shoot = rand() % 15;
		}
		bullets.push_back(new Bullet(aliens[shoot]->getX(), aliens[shoot]->getY(), aliens[shoot]));
		shoot_interval = 2.0;
	}
	if (move_interval <= 0.0)
	{
		for (int i = 0; i < aliens.size(); i++)
		{
			if (aliens[i]->isAlive())
			{
				aliens[i]->move_y(-elapsed * 50.0);
			}
		}
		move_interval = 5.0;
	}

	checkCollisions();

	//Player Loses
	if (!player->isAlive())
		state = GAME_OVER;
	for (int i = aliens.size() - 1; i >= 0; i--)
	{
		if (aliens[i]->isAlive())
		{
			if ((aliens[i]->getY() - aliens[i]->getHeight() / 2.0) < (player->getY() + player->getHeight() / 2.0))
			{
				state = GAME_OVER;
				break;
			}
		}
	}
	//Player Wins
	if (aliensDead())
	{
		state = GAME_OVER;
		win = true;
	}
}

void GameApp::RenderMainMenu()
{
	modelMatrix.identity();
	modelMatrix.Translate(-10.0, 3.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Space Invaders", 2.5, -1.0);

	modelMatrix.identity();
	modelMatrix.Translate(-5.0, 0.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press ENTER", 1.0, 0.0);
}

void GameApp::RenderGame()
{
	//Render Player
	float x = player->getX();
	float y = player->getY();
	float h = player->getHeight() / 2.0;
	float w = player->getWidth() / 2.0;

	float playerCoords[] = { x - w, y - h, x + w, y + h, x - w, y + h, x - w, y - h, x + w, y - h, x + w, y + h };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, playerCoords);
	glEnableVertexAttribArray(program->positionAttribute);

	float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	modelMatrix.identity();
	program->setModelMatrix(modelMatrix);

	glBindTexture(GL_TEXTURE_2D, playerID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	//Render Aliens
	for (int i = 0; i < aliens.size(); i++)
	{
		if (aliens[i]->isAlive())
		{
			if (i < 5)
				DrawSpriteSheetSprite(0, 2, 3, aliens[i]->getX(), aliens[i]->getY());
			else if (i < 10)
				DrawSpriteSheetSprite(2, 2, 3, aliens[i]->getX(), aliens[i]->getY());
			else
				DrawSpriteSheetSprite(4, 2, 3, aliens[i]->getX(), aliens[i]->getY());
		}
	}

	//Render Bullets
	for (int i = 0; i < bullets.size(); i++)
	{
		float x = bullets[i]->getXposition();
		float y = bullets[i]->getYposition();

		float bullCoords[] = { x - 0.25, y - 0.25, x + 0.25, y + 0.25, x - 0.25, y + 0.25, x - 0.25, y - 0.25, x + 0.25, y - 0.25, x + 0.25, y + 0.25 };

		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, bullCoords);
		glEnableVertexAttribArray(program->positionAttribute);

		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program->texCoordAttribute);

		modelMatrix.identity();
		program->setModelMatrix(modelMatrix);

		glBindTexture(GL_TEXTURE_2D, bulletID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);
	}
}

void GameApp::RenderGameOver()
{
	if (win)
	{
		modelMatrix.identity();
		modelMatrix.Translate(-5.5, 3.0, 0.0);
		program->setModelMatrix(modelMatrix);
		DrawText(fontID, "You Win", 2.5, -1.0);
	}
	else
	{
		modelMatrix.identity();
		modelMatrix.Translate(-5.5, 3.0, 0.0);
		program->setModelMatrix(modelMatrix);
		DrawText(fontID, "Game Over", 2.5, -1.0);
	}
}

void GameApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state)
	{
	case MAIN_MENU:
		RenderMainMenu();
		break;
	case GAME_LEVEL:
		RenderGame();
		break;
	case GAME_OVER:
		RenderGameOver();
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool GameApp::UpdateAndRender()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	Update(elapsed);
	Render();
	return done;
}

bool GameApp::aliensDead()
{
	for (int i = 0; i < aliens.size(); i++)
		if (aliens[i]->isAlive())
			return false;
	return true;
}

