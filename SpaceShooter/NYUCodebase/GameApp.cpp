#include "GameApp.h"

GameApp::GameApp() : done(false)
{
	Setup();
}
GameApp::~GameApp() 
{ 
	Mix_FreeMusic(menuMusic);
	Mix_FreeMusic(gameMusic);
	SDL_Quit();
}

// SETUP
void GameApp::Setup()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);

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

	state = MAIN_MENU;
	stateSwitched = true;

	fontID = LoadTexture("font1.png");
	playerID = LoadTexture("ship.png");
	asteroidID = LoadTexture("asteroid.png");
	laserID = LoadTexture("laserBlue04.png");
	gruntID = LoadTexture("ufo.png");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	menuMusic = Mix_LoadMUS("title.mp3");
	gameMusic = Mix_LoadMUS("corneria.mp3");

	laserSound = Mix_LoadWAV("laserSound.wav");
	explosionSound = Mix_LoadWAV("explosion.wav");

	LoadCutsceneVar();
}

//	UPDATE
void GameApp::Update(float elapsed)
{
	ProcessEvents(elapsed);
	if (state == MAIN_MENU)
		UpdateMainMenu();
	if (state == CUTSCENE_1)
		UpdateCutscene1(elapsed);
	if (state == LEVEL_1 || state == LEVEL_2 || state == LEVEL_3)
		UpdateLevel1(elapsed);
	if (state == CUTSCENE_2)
		UpdateCutscene2(elapsed);
	if (state == CUTSCENE_3)
		UpdateCutscene3(elapsed);
	if (state == CUTSCENE_4)
		UpdateCutscene4(elapsed);
	if (state == CUTSCENE_5)
		UpdateCutscene5(elapsed);
	if (state == CUTSCENE_6)
		UpdateCutscene6(elapsed);
}
void GameApp::UpdateMainMenu()
{
	if (stateSwitched)
	{
		changeMusic(menuMusic);
		stateSwitched = false;
	}
}
void GameApp::ProcessEvents(float elapsed)
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}

		const Uint8* keys = SDL_GetKeyboardState(NULL);

		if (state == MAIN_MENU)
		{
			if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_RETURN2])
			{
				generatePlayer();
				state = CUTSCENE_1;
				stateSwitched = true;
			}
			if (keys[SDL_SCANCODE_ESCAPE])
			{
				done = true;
			}
		}
		else if (state == LEVEL_1 || state == LEVEL_2 || state == LEVEL_3)
		{
			// MOVEMENT
			if (keys[SDL_SCANCODE_LEFT])
			{
				if (player->x >= -10.0)
					player->vel_x = -7.5;
			}
			if (keys[SDL_SCANCODE_RIGHT])
			{
				if (player->x <= 10.0)
					player->vel_x = 7.5;
			}
			// SHOOTING
			if (keys[SDL_SCANCODE_SPACE])
			{
				if (player->shootCooldown <= 0.0)
				{
					player->shoot(bullets, laserID);
					Mix_PlayChannel(-1, laserSound, 0);
					player->shootCooldown = 0.75;
				}
			}
		}
		else if (state == GAME_OVER)
		{
			if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_RETURN2])
			{
				state = MAIN_MENU;
				stateSwitched = true;
			}
			if (keys[SDL_SCANCODE_ESCAPE])
			{
				done = true;
			}
		}
		else if (state == GAME_WIN)
		{
			if (keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_RETURN2])
			{
				state = MAIN_MENU;
				stateSwitched = true;
			}
			if (keys[SDL_SCANCODE_ESCAPE])
			{
				done = true;
			}
		}
	}
}
void GameApp::UpdateLevel1(float elapsed)
{
	if (stateSwitched)
	{
		// CREATE ENEMIES
		generateEnemies();
		stateSwitched = false;
	}

	// UPDATE AND MOVE PLAYER
	player->Update(elapsed);
	player->move_x(elapsed);

	//UPDATE AND MOVE ENEMIES
	for (int i = 0; i < level1Enemies.size(); i++)
	{
		level1Enemies[i]->Update(elapsed, player);
		level1Enemies[i]->move_y(elapsed);
		if (level1Enemies[i]->type == GRUNT)
		{
			if (at_x(level1Enemies[i], player->x))
			{
				if (level1Enemies[i]->shootCooldown <= 0.0 && level1Enemies[i]->willShoot)
				{
					level1Enemies[i]->shoot(bullets, laserID);
					level1Enemies[i]->shootCooldown = 3.0;
				}
			}
		}
		level1Enemies[i]->expires();
	}

	// UPDATE AND MOVE BULLETS
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->live)
		{
			bullets[i]->Update(elapsed);
			bullets[i]->move_y(elapsed);
		}
	}

	// CHECK COLLISIONS
	bulletToEnemy();
	checkPlayerCollision();
	cleanEnemies();
	cleanBullets();
	checkGameOver();

	if (state == LEVEL_1 && levelComplete())
	{
		state = CUTSCENE_2;
		stateSwitched = true;
	}
	if (state == LEVEL_2 && levelComplete())
	{
		state = CUTSCENE_4;
		stateSwitched = true;
	}
	if (state == LEVEL_3 && levelComplete())
	{
		state = CUTSCENE_6;
		stateSwitched = true;
	}
}
void GameApp::UpdateCutscene1(float elapsed)
{
	// CHANGE MUSIC & SETUP CUTSCENE VAR
	if (stateSwitched)
	{
		LoadCutsceneVar();
		changeMusic(gameMusic);
		stateSwitched = false;
	}
	
	// CUTSCENE CONTROL
	if (up)
	{
		level_y += elapsed * 10.0;
		if (level_y >= 3.0)
			up = false;
	}
	else if (!up && !down)
	{
		level_cd -= elapsed * 5.0;
		if (level_cd <= 0.0)
			down = true;
	}
	else
	{
		level_y -= elapsed * 10;
		if (level_y < -15.0)
		{
			state = LEVEL_1;
			stateSwitched = true;
		}
	}
}
void GameApp::UpdateCutscene2(float elapsed)
{
	// SETUP CUTSCENE VAR
	if (stateSwitched)
	{
		LoadCutsceneVar();
		stateSwitched = false;
	}

	// CUTSCENE CONTROL
	if (up)
	{
		level_y2 += elapsed * 10.0;
		if (level_y2 >= 3.0)
			up = false;
	}
	else if (!up && !down)
	{
		level_cd -= elapsed * 5.0;
		if (level_cd <= 0.0)
			down = true;
	}
	else
	{
		level_y2 -= elapsed * 10;
		if (level_y2 < -15.0)
		{
			state = CUTSCENE_3;
			stateSwitched = true;
		}
	}
}
void GameApp::UpdateCutscene3(float elapsed)
{
	// CHANGE MUSIC & SETUP CUTSCENE VAR
	if (stateSwitched)
	{
		LoadCutsceneVar();
		stateSwitched = false;
	}

	// CUTSCENE CONTROL
	if (up)
	{
		level_y += elapsed * 10.0;
		if (level_y >= 3.0)
			up = false;
	}
	else if (!up && !down)
	{
		level_cd -= elapsed * 5.0;
		if (level_cd <= 0.0)
			down = true;
	}
	else
	{
		level_y -= elapsed * 10;
		if (level_y <= -15.0)
		{
			state = LEVEL_2;
			stateSwitched = true;
		}
	}
}
void GameApp::UpdateCutscene4(float elapsed)
{
	// SETUP CUTSCENE VAR
	if (stateSwitched)
	{
		LoadCutsceneVar();
		stateSwitched = false;
	}

	// CUTSCENE CONTROL
	if (up)
	{
		level_y2 += elapsed * 10.0;
		if (level_y2 >= 3.0)
			up = false;
	}
	else if (!up && !down)
	{
		level_cd -= elapsed * 5.0;
		if (level_cd <= 0.0)
			down = true;
	}
	else
	{
		level_y2 -= elapsed * 10;
		if (level_y2 < -15.0)
		{
			state = CUTSCENE_5;
			stateSwitched = true;
		}
	}
}
void GameApp::UpdateCutscene5(float elapsed)
{
	// CHANGE MUSIC & SETUP CUTSCENE VAR
	if (stateSwitched)
	{
		LoadCutsceneVar();
		stateSwitched = false;
	}

	// CUTSCENE CONTROL
	if (up)
	{
		level_y += elapsed * 10.0;
		if (level_y >= 3.0)
			up = false;
	}
	else if (!up && !down)
	{
		level_cd -= elapsed * 5.0;
		if (level_cd <= 0.0)
			down = true;
	}
	else
	{
		level_y -= elapsed * 10;
		if (level_y < -15.0)
		{
			state = LEVEL_3;
			stateSwitched = true;
		}
	}
}
void GameApp::UpdateCutscene6(float elapsed)
{
	// SETUP CUTSCENE VAR
	if (stateSwitched)
	{
		LoadCutsceneVar();
		stateSwitched = false;
	}

	// CUTSCENE CONTROL
	if (up)
	{
		level_y2 += elapsed * 10.0;
		if (level_y2 >= 3.0)
			up = false;
	}
	else if (!up && !down)
	{
		level_cd -= elapsed * 5.0;
		if (level_cd <= 0.0)
			down = true;
	}
	else
	{
		level_y2 -= elapsed * 10;
		if (level_y2 < -15.0)
		{
			state = GAME_WIN;
			stateSwitched = true;
		}
	}
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

//	RENDER
void GameApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (state == MAIN_MENU)
		RenderMainMenu();
	else if (state == CUTSCENE_1)
		RenderCutScene1();
	else if (state == LEVEL_1 || state == LEVEL_2 || state == LEVEL_3)
		RenderGameLevel();
	else if (state == CUTSCENE_2)
		RenderCutscene2();
	else if (state == CUTSCENE_3)
		RenderCutscene3();
	else if (state == CUTSCENE_4)
		RenderCutscene4();
	else if (state == CUTSCENE_5)
		RenderCutscene5();
	else if (state == CUTSCENE_6)
		RenderCutscene6();
	else if (state == GAME_OVER)
		RenderGameOver();
	else if (state == GAME_WIN)
		RenderGameWin();
	SDL_GL_SwapWindow(displayWindow);
}
void GameApp::RenderMainMenu()
{
	modelMatrix.identity();
	modelMatrix.Translate(-12.0, 3.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Space Shooter", 3.5, -1.5);

	modelMatrix.identity();
	modelMatrix.Translate(-5.0, -1.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press Enter", 1.5, -0.5);

	modelMatrix.identity();
	modelMatrix.Translate(-8.0, -3.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Press ESC to quit", 1.5, -0.5);
}
void GameApp::RenderCutScene1()
{
	player->Render(program, modelMatrix);

	modelMatrix.identity();
	modelMatrix.Translate(level_x, level_y, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Level 1", 3.5, -1.5);
}
void GameApp::RenderGameLevel()
{
	player->Render(program, modelMatrix);
	renderEnemies();
	renderBullets();
}
void GameApp::RenderGameOver()
{
	modelMatrix.identity();
	modelMatrix.Translate(-8.0, 3.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Game Over", 3.5, -1.5);
}
void GameApp::RenderCutscene2()
{
	player->Render(program, modelMatrix);

	modelMatrix.identity();
	modelMatrix.Translate(level_x2, level_y2, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Level 1 Complete", 3.5, -2.0);
}
void GameApp::RenderCutscene3()
{
	player->Render(program, modelMatrix);

	modelMatrix.identity();
	modelMatrix.Translate(level_x, level_y, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Level 2", 3.5, -1.5);
}
void GameApp::RenderCutscene4()
{
	player->Render(program, modelMatrix);

	modelMatrix.identity();
	modelMatrix.Translate(level_x2, level_y2, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Level 2 Complete", 3.5, -2.0);
}
void GameApp::RenderCutscene5()
{
	player->Render(program, modelMatrix);

	modelMatrix.identity();
	modelMatrix.Translate(level_x, level_y, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Level 3", 3.5, -1.5);
}
void GameApp::RenderCutscene6()
{
	player->Render(program, modelMatrix);

	modelMatrix.identity();
	modelMatrix.Translate(level_x2, level_y2, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "Level 3 Complete", 3.5, -2.0);
}
void GameApp::RenderGameWin()
{
	modelMatrix.identity();
	modelMatrix.Translate(-6.0, 3.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(fontID, "You Won", 3.5, -1.5);
}

// GAME TOOLS
void GameApp::generatePlayer()
{
	player = new Entity(0.0, -7.0, 2.0, 2.0, playerID);
	player->type = PLAYER;
	player->vel_x = 0;
	player->vel_y = 0;
	player->acc_x = 0;
	player->acc_y = 0;
	player->shootCooldown = 0.0;
}
void GameApp::generateEnemies()
{
	if (state == LEVEL_1)
	{
		for (int i = 0; i < 1; i++)
		{
			Entity* e = createAsteroid();
			if (canBePlaced(e, level1Enemies))
				level1Enemies.push_back(e);
			else
				--i;
		}
	}
	if (state == LEVEL_2)
	{
		for (int i = 0; i < 1; i++)
		{
			Entity* e = createGrunt();
			if (canBePlaced(e, level1Enemies))
				level1Enemies.push_back(e);
			else
				--i;
		}
	}
	if (state == LEVEL_3)
	{
		srand(SDL_GetTicks());
		int numA = rand() % 25 + 1;
		int numG = 50 - numA;
		for (int i = 0; i < 1; i++)
		{
			Entity* e = createAsteroid();
			if (canBePlaced(e, level1Enemies))
				level1Enemies.push_back(e);
			else
				--i;
		}
		for (int i = 0; i < 1; i++)
		{
			Entity* e = createGrunt();
			if (canBePlaced(e, level1Enemies))
				level1Enemies.push_back(e);
			else
				--i;
		}
	}
}
Entity* GameApp::createAsteroid()
{
	srand(SDL_GetTicks());
	float x = rand() % 21 - 10;
	float xd = rand() % 100;
	x += xd / 100.0;
	float y = rand() % 21 + 10;
	float yd = rand() % 100;
	y += yd / 100.0;
	float s = rand() % 3 + 1;
	float sd = rand() % 100;
	s += sd / 100.0;
	Entity* e = new Entity(x, y, s, s, asteroidID);
	e->type = ASTEROID;

	float v = rand() % 3 + 1;
	float vd = rand() % 100;
	e->vel_y = -(v + vd / 100.0);

	return e;
}
Entity* GameApp::createGrunt()
{
	srand(SDL_GetTicks());
	float x = rand() % 21 - 10;
	float xd = rand() % 100;
	x += xd / 100.0;
	float y = rand() % 21 + 10;
	float yd = rand() % 100;
	y += yd / 100.0;
	float s = 2.5;
	Entity* e = new Entity(x, y, s, s, gruntID);
	e->type = GRUNT;
	e->shootCooldown = 0.0;
	e->vel_y = -2.0;
	e->willShoot = false;
	return e;
}
void GameApp::renderEnemies()
{
	for (int i = 0; i < level1Enemies.size(); i++)
	{
		if (level1Enemies[i]->alive)
		{
			level1Enemies[i]->Render(program, modelMatrix);
		}
	}
}
void GameApp::renderBullets()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->live)
			bullets[i]->Render(program, modelMatrix);
	}
}
bool GameApp::canBePlaced(Entity* e, std::vector<Entity*> v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (collides(e, v[i]))
			return false;
	}
	return true;
}
bool GameApp::collides(Entity* e1, Entity* e2)
{
	if (!e1->alive || !e2->alive)
		return false;
	// CHECK TOP
	if (e1->y + e1->height / 2.0 < e2->y - e2->height / 2.0 ||
		// CHECK BOTTOM
		e1->y - e1->height / 2.0 > e2->y + e2->height / 2.0 ||
		//CHECK LEFT
		e1->x - e1->width / 2.0 > e2->x + e2->width / 2.0 ||
		// CHECK RIGHT
		e1->x + e1->width / 2.0 < e2->x - e2->width / 2.0)
		return false;

	return true;
}
bool GameApp::bulletCollides(Entity* e, Bullet* b)
{
	if (e->type == GRUNT && b->vel_y < 0.0)
		return false;
	if (e->type == PLAYER && b->vel_y > 0.0)
		return false;
	if (!b->live)
		false;
	if (!e->alive)
		return false;
	// CHECK TOP
	if (e->y + e->height / 2.0 < b->y - BULLET_HEIGHT / 2.0 ||
		// CHECK BOTTOM
		e->y - e->height / 2.0 > b->y + BULLET_HEIGHT / 2.0 ||
		//CHECK LEFT
		e->x - e->width / 2.0 > b->x + BULLET_WIDTH / 2.0 ||
		// CHECK RIGHT
		e->x + e->width / 2.0 < b->x - BULLET_WIDTH / 2.0)
		return false;

	return true;
}
void GameApp::bulletToEnemy()
{
	for (int i = 0; i < level1Enemies.size(); i++)
	{
		for (int j = 0; j < bullets.size(); j++)
		{
			if (bulletCollides(level1Enemies[i], bullets[j]))
			{
				// REMOVE BULLET
				bullets[j]->live = false;

				// REMOVE ASTEROID
				level1Enemies[i]->alive = false;

				Mix_PlayChannel(-1, explosionSound, 0);
			}
		}
	}
	cleanBullets();
}
void GameApp::cleanBullets()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (!bullets[i]->live)
		{
			bullets.erase(bullets.begin() + i);
			--i;
		}
	}
}
void GameApp::cleanEnemies()
{
	for (int i = 0; i < level1Enemies.size(); i++)
	{
		if (!level1Enemies[i]->alive)
		{
			level1Enemies.erase(level1Enemies.begin() + i);
			--i;
		}
	}
}
bool GameApp::levelComplete()
{
	if (level1Enemies.size() == 0)
		return true;
	return false;
}
void GameApp::setUpCutscene()
{
	cutscene_done = false;
	up = true;
	down = false;
}
void GameApp::LoadCutsceneVar()
{
	level_x = -6.0;
	level_y = -15.0;
	level_x2 = -10.0;
	level_y2 = -15.0;
	level_cd = 3.0;
	cutscene_done = false;
	up = true;
	down = false;
}
void GameApp::checkPlayerCollision()
{
	for (int i = 0; i < level1Enemies.size(); i++)
	{
		if (collides(player, level1Enemies[i]))
		{
			player->alive = false;
			break;
		}
	}
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bulletCollides(player, bullets[i]))
		{
			player->alive = false;
			break;
		}
	}
}
void GameApp::checkGameOver()
{
	if (!player->alive)
	{
		state = GAME_OVER;
	}
}
bool GameApp::at_x(Entity* e, float goal)
{
	return e->x <= goal + 0.5 && e->x >= goal - 0.5;
}

//	UTILITY
void GameApp::DrawText(int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
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
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
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
void GameApp::changeMusic(Mix_Music *m)
{
	Mix_PlayMusic(m, -1);
}