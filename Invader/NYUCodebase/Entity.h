#pragma once

enum ENTITY_TYPE {PLAYER, ALIEN};

class Entity
{
public:
	Entity(float x, float y, float h, float w, ENTITY_TYPE type);

	//Accessors
	float getX() const;
	float getY() const;
	
	float getHeight() const;
	float getWidth() const;

	bool isAlive() const;

	ENTITY_TYPE getType() const;

	void move_x(float elapsed);
	void move_y(float elapsed);
	void dies();

private:
	float original_x;
	float original_y;

	float height;
	float width;

	ENTITY_TYPE type;

	bool alive;
};

