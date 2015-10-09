#include "GameApp.h"

GameApp::GameApp() : done(false)
{
	player = new Entity(0.0, 0.0);
	state = MAIN_MENU;
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
}

void GameApp::ProcessEvents()
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
				if (player->getXposition() > -14.0)
					player->move_x(-1.0);
			}
			else if (keys[SDL_SCANCODE_RIGHT])
			{
				if (player->getXposition() < 14.0)
					player->move_x(1.0);
			}

			if (keys[SDL_SCANCODE_SPACE])
			{
				bullets.push_back(new Projectile(player->getXposition(), player->getYposition(), 0.01));
			}
			break;
		}
	}
}

void GameApp::Update()
{
	ProcessEvents();
}

void GameApp::RenderMainMenu()
{
	modelMatrix.identity();
	modelMatrix.Translate(-10.0, 3.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(LoadTexture("font1.png"), "Space Invaders", 2.5, -1.0);

	modelMatrix.identity();
	modelMatrix.Translate(-5.0, 0.0, 0.0);
	program->setModelMatrix(modelMatrix);
	DrawText(LoadTexture("font1.png"), "Press ENTER", 1.0, 0.0);
}

void GameApp::RenderGame()
{
	//Render Player
	float playerCoords[] = { -2.0, -8.0, 2.0, -6.0, -2.0, -6.0, -2.0, -8.0, 2.0, -8.0, 2.0, -6.0 };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, playerCoords);
	glEnableVertexAttribArray(program->positionAttribute);

	float texCoords2[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
	glEnableVertexAttribArray(program->texCoordAttribute);

	modelMatrix.identity();
	modelMatrix.Translate(player->getXposition(), 0.0, 0.0);
	program->setModelMatrix(modelMatrix);

	glBindTexture(GL_TEXTURE_2D, LoadTexture("ship.png"));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
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
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool GameApp::UpdateAndRender()
{
	Update();
	Render();
	return done;
}

