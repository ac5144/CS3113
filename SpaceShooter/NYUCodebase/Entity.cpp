#include "Entity.h"

Entity::Entity(float x, float y, float h, float w, GLuint id) : x(x), y(y), height(h), width(w), textureID(id)
{
}

Entity::~Entity()
{
}

void Entity::Render(ShaderProgram *program, Matrix& modelMatrix)
{
	float vertices[] = {
		x - width / 2.0, y - height / 2.0,
		x + width / 2.0, y + height / 2.0,
		x - width / 2.0, y + height / 2.0,
		x - width / 2.0, y - height / 2.0,
		x + width / 2.0, y - height / 2.0,
		x + width / 2.0, y + height / 2.0 };

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
