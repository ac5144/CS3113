#pragma once
class Ball
{
public:
	Ball();

	float getPositionX() const;
	float getPositionY() const;

	void move(float elapsed);
	void reset();

	void inverse_X();
	void inverse_Y();

private:
	//DIMENSION COORDINATES
	float top;
	float bot;
	float left;
	float right;

	//MOVEMENT
	float move_X;
	float move_Y;

	//POSITION
	float position_Y;
	float position_X;
};

