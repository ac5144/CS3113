#include "PongApp.h"

PongApp::PongApp() : done(false), winner(0)
{
	PongApp::Setup();
	PongApp::leftPaddle = new Paddle(1.0, -1.0, -8.0, -7.5);
	PongApp::rightPaddle = new Paddle(1.0, -1.0, 7.5, 8.0);
	PongApp::gameBall = new Ball();
}

PongApp::~PongApp()
{
	SDL_Quit();
}

void PongApp::Setup()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);

	program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	projectionMatrix.setOrthoProjection(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);

	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	glUseProgram(program->programID);
}

void PongApp::ProcessEvents()
{
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}

		const Uint8* keys = SDL_GetKeyboardState(NULL);

		if (keys[SDL_SCANCODE_UP])
			//RIGHT PADDLE UP
			rightPaddle->moveUp(3.5);
		else if (keys[SDL_SCANCODE_DOWN])
			//RIGHT PADDLE DOWN
			rightPaddle->moveDown(-3.5);

		if (keys[SDL_SCANCODE_W])
			//LEFT PADDLE UP
			leftPaddle->moveUp(3.5);
		else if (keys[SDL_SCANCODE_S])		
			//LEFT PADDLE DOWN
			leftPaddle->moveDown(-3.5);
	}
}

void PongApp::Update()
{
	float BallXDir = gameBall->getPositionX();
	float BallYDir = gameBall->getPositionY();

	if (BallXDir >= rightPaddle->getLeftCoord() && BallYDir >= rightPaddle->getBotCoord() && BallYDir <= rightPaddle->getTopCoord() ||
		BallXDir <= leftPaddle->getRightCoord() && BallYDir >= leftPaddle->getBotCoord() && BallYDir <= leftPaddle->getTopCoord())
		gameBall->inverse_X();

	if (BallYDir >= 4.5 || BallYDir <= -4.5)
		gameBall->inverse_Y();

	gameBall->move();

	BallXDir = gameBall->getPositionX();
	BallYDir = gameBall->getPositionY();

	if (BallXDir < -8.0)
	{
		winner = 1;
		gameBall->reset();
	}

	if (BallXDir > 8.0)
	{
		winner = -1;
		BallXDir = 0.0;
		BallYDir = 0.0;
		gameBall->reset();
	}
}

void PongApp::Render()
{
	glClearColor(0.0f, 0.0f, 0.2f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw ball
	float ballCoords[] = { -0.25f, 0.25f, -0.25f, -0.25f, 0.25, -0.25f, -0.25f, 0.25f, 0.25, -0.25f, 0.25f, 0.25f};
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, ballCoords);
	glEnableVertexAttribArray(program->positionAttribute);

	modelMatrix.identity();
	modelMatrix.Translate(gameBall->getPositionX(), gameBall->getPositionY(), 0.0);
	program->setModelMatrix(modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);

	//Draw L Paddle
	float paddleLCoords[] = { -8.0f, 1.0f, -8.0f, -1.0f, -7.5f, -1.0f, -8.0f, 1.0f, -7.5f, -1.0f, -7.5f, 1.0f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, paddleLCoords);
	glEnableVertexAttribArray(program->positionAttribute);

	modelMatrix.identity();
	modelMatrix.Translate(0.0, leftPaddle->getPositionY(), 0.0);
	program->setModelMatrix(modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);

	//Draw R Paddle
	float paddleRCoords[] = { 8.0f, 1.0f, 8.0f, -1.0f, 7.5f, -1.0f, 8.0f, 1.0f, 7.5f, -1.0f, 7.5f, 1.0f };
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, paddleRCoords);
	glEnableVertexAttribArray(program->positionAttribute);

	modelMatrix.identity();
	modelMatrix.Translate(0.0, rightPaddle->getPositionY(), 0.0);
	program->setModelMatrix(modelMatrix);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);

	//Draw Winner
	if (winner != 0)
	{
		float point[] = { 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.0f };
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, point);
		glEnableVertexAttribArray(program->positionAttribute);

		modelMatrix.identity();
		if (winner == 1)
			modelMatrix.Translate(6.0, -2.0, 0.0);
		else
			modelMatrix.Translate(-6.0, -2.0, 0.0);
		program->setModelMatrix(modelMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(program->positionAttribute);
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool PongApp::UpdateAndRender()
{
	ProcessEvents();
	Update();
	Render();

	return done;
}
