#include "Entity.h"


Entity::Entity(unsigned int textureID) : x(0.0), y(0.0), height(6.0), width(5.0)
{
	sprite = SheetSprite(textureID, width, height, 5.0);
	velocity_y = 0.0;
	acceleration_y = -1.0;
}


float Entity::getX() const { return x; }
float Entity::getY() const { return y; }

float Entity::getHeight() const { return height; }
float Entity::getWidth() const { return width; }

void Entity::Render(ShaderProgram *program, Matrix& modelMatrix)
{
	sprite.DrawSpriteSheetSprite(8, 8, 4, program, modelMatrix, x, y);
	
}

void Entity::Update(float elapsed)
{
	y += velocity_y * elapsed;
	velocity_y += acceleration_y * elapsed;
}
