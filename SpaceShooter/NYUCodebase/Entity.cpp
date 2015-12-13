#include "Entity.h"

Entity::Entity(float x, float y, float h, float w, GLuint id) : x(x), y(y), height(h), width(w), textureID(id), alive(true)
{
}

Entity::~Entity()
{
}

// UPDATE
void Entity::Update(float elapsed)
{
	vel_x = lerp(vel_x, 0.0, elapsed * 2.0);
	vel_x += acc_x * elapsed;

	if (type == PLAYER)
	{
		if (shootCooldown > 0.0)
		{
			shootCooldown -= elapsed;
		}
	}
}

// MOVEMENT
void Entity::move_x(float elapsed)
{
	x += vel_x * elapsed;
}
void Entity::move_y(float elapsed)
{
	y += vel_y * elapsed;
}

// RENDER
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

// GAME TOOLS
void Entity::expires()
{
	if (type == ASTEROID)
		if (y <= -15.0)
			alive = false;
}
void Entity::shoot(std::vector<Bullet*>& v, GLuint id)
{
	Bullet* b = new Bullet(id);
	b->x = x;
	b->y = y + height / 2.0 + 0.1;
	b->vel_y = 10.0;
	b->vel_x = 0.0;

	v.push_back(b);
}


// UTILITY
float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}
