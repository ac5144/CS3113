#include "Bullet.h"

Bullet::Bullet(GLuint id) : textureID(id)
{
}

Bullet::~Bullet()
{
}

// UPDATE
void Bullet::Update(float elapsed)
{
	if (y >= 11.0 && vel_y > 0.0)
		live = false;
	if (y <= -12 && vel_y < 0.0)
		live = false;
}

// MOVEMENT
void Bullet::move_y(float elapsed)
{
	y += vel_y * elapsed;
}

// RENDER
void Bullet::Render(ShaderProgram *program, Matrix& modelMatrix)
{
	float vertices[] = {
		x - BULLET_WIDTH / 2.0, y - BULLET_HEIGHT / 2.0,
		x + BULLET_WIDTH / 2.0, y + BULLET_HEIGHT / 2.0,
		x - BULLET_WIDTH / 2.0, y + BULLET_HEIGHT / 2.0,
		x - BULLET_WIDTH / 2.0, y - BULLET_HEIGHT / 2.0,
		x + BULLET_WIDTH / 2.0, y - BULLET_HEIGHT / 2.0,
		x + BULLET_WIDTH / 2.0, y + BULLET_HEIGHT / 2.0 };

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	float texCoords[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f };

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	modelMatrix.identity();
	program->setModelMatrix(modelMatrix);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}