#include "Ball.h"

Ball::Ball() : move_X(0.0015), move_Y(0.0015), position_X(0.0), position_Y(0.0)
{
}

float Ball::getPositionX() const { return position_X; }
float Ball::getPositionY() const { return position_Y; }

void Ball::move(float elapsed)
{
	position_X += move_X * elapsed * 2000.0;
	position_Y += move_Y * elapsed * 2000.0;
}

void Ball::reset()
{
	position_X = 0.0;
	position_Y = 0.0;
}

void Ball::inverse_X()
{
	move_X *= -1.0;
}

void Ball::inverse_Y()
{
	move_Y *= -1.0;
}
