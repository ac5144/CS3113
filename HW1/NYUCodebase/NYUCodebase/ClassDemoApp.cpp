#include "ClassDemoApp.h"


ClassDemoApp::ClassDemoApp()
{
	Setup();
}

ClassDemoApp::~ClassDemoApp()
{
	SDL_Quit();
}

void ClassDemoApp::Setup()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	glUseProgram(program->programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
}

void ClassDemoApp::Update()
{

}

void ClassDemoApp::UpdateAndRender()
{
	SDL_Event event;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				float unitX = (((float)event.motion.x / 640.0f) * 3.554) - 1.777f;
				float unitY = (((float)(360 - event.motion.y) / 360.0f) * 2.0) - 1.0f;

				positionX = unitX;
				positionY = unitY;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				angle += 3.1415926535 * 0.25;
			}
		}

		glClearColor(0.4f, 0.2f, 0.4f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);
		/*
		//Finn the Human
		GLuint img1 = LoadTexture("finn.png");
		float vertices[] = { -0.5f, -1.0f, 0.5f, 1.0f, -0.5f, 1.0f, 0.5f, 1.0f, -0.5f, -1.0f, 0.5f, -1.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glBindTexture(GL_TEXTURE_2D, img1);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Rick and Morty
		GLuint img2 = LoadTexture("RickandMorty.png");
		float vertices2[] = { -3.5f, -2.0f, -0.5f, 0.5f, -3.5f, 0.5f, -0.5f, 0.5f, -3.5, -2.0f, -0.5f, -2.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords2[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glBindTexture(GL_TEXTURE_2D, img2);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//Steven
		GLuint img3 = LoadTexture("steven.png");
		float vertices3[] = { 1.0f, -1.0f, 3.0f, 1.0f, 1.0f, 1.0f, 3.0f, 1.0f, 1.0f, -1.0f, 3.0, -1.0f };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
		glEnableVertexAttribArray(program.positionAttribute);

		float texCoords3[] = { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
		glEnableVertexAttribArray(program.texCoordAttribute);

		glBindTexture(GL_TEXTURE_2D, img3);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);
		glDisableVertexAttribArray(program.texCoordAttribute);

		//float ticks = (float)SDL_GetTicks() / 1000.0f;
		//float elapsed = ticks - lastFrameTicks;
		//lastFrameTicks = ticks;
		//angle += elapsed * 2.0 * 3.14;
		modelMatrix.identity();
		modelMatrix.Rotate(angle);
		//modelMatrix.Translate(positionX, positionY, 0.0);
		program.setModelMatrix(modelMatrix);
		*/
		SDL_GL_SwapWindow(displayWindow);
	}
}