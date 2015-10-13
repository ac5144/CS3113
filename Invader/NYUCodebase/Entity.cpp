#include "Entity.h"
#include "Bullet.h"

Entity::Entity(float x, float y, float h, float w, ENTITY_TYPE type) : position_x(x), position_y(y), height(h), width(w), type(type), alive(true)
{
}

float Entity::getXposition() const { return position_x; }
float Entity::getYposition() const { return position_y; }
bool Entity::isAlive() const { return alive; }
ENTITY_TYPE Entity::getType() const { return type; };
float Entity::getHeight() const { return height; }
float Entity::getWidth() const { return width; }

void Entity::move_x(float x) { position_x += x; }
void Entity::move_y(float y) { position_y += y; }
void Entity::shoot(std::vector<Bullet*>& v){ v.push_back(new Bullet(position_x, position_y, this)); }
void Entity::dies() { alive = false; }
void Entity::draw(GLuint textureID, ShaderProgram* program, Matrix& modelMatrix)
{
	float vertices[] = { 
		position_x - width / 2.0, position_y - height / 2.0, 
		position_x + width / 2.0, position_y + height / 2.0, 
		position_x - width / 2.0, position_y + height / 2.0, 
		position_x - width / 2.0, position_y - height / 2.0,
		position_x + width / 2.0, position_y - height / 2.0,
		position_x + width / 2.0, position_y + height / 2.0 };

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

