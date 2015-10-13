#include "Bullet.h"

Bullet::Bullet(float x, float y, Entity* source) : position_x(x), position_y(y), source(source), ttl(1.5)
{
	if (source->getType() == PLAYER)
		position_y += 1.0;
	else
		position_y -= 1.0;
}

void Bullet::move_y(float elapsed)
{
	if (source->getType() == PLAYER)
		position_y += elapsed * 10.0;
	else
		position_y -= elapsed * 10.0;

	ttl -= elapsed;
}

float Bullet::getXposition() const { return position_x; }
float Bullet::getYposition() const { return position_y; }
float Bullet::getTTL() const { return ttl; }
Entity* Bullet::getSource() { return source; }

void Bullet::draw(GLuint textureID, ShaderProgram* program, Matrix& modelMatrix)
{
	float vertices[] = { 
		position_x - BULLET_WIDTH / 2.0, position_y - BULLET_HEIGHT / 2.0,
		position_x + BULLET_WIDTH / 2.0, position_y + BULLET_HEIGHT / 2.0,
		position_x - BULLET_WIDTH / 2.0, position_y + BULLET_HEIGHT / 2.0,
		position_x - BULLET_WIDTH / 2.0, position_y - BULLET_HEIGHT / 2.0,
		position_x + BULLET_WIDTH / 2.0, position_y - BULLET_HEIGHT / 2.0,
		position_x + BULLET_WIDTH / 2.0, position_y + BULLET_HEIGHT / 2.0 };

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
