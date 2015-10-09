#pragma once

class Entity
{
public:
	Entity(float x, float y);
	void move_x(float x);
	void move_y(float y);
	float getXposition() const;
	float getYposition() const;
	bool isAlive() const;

private:
	float position_x;
	float position_y;
	bool alive;
};

