#include "Paddle.h"


Paddle::Paddle(float t, float b, float l, float r) : top(t), bot(b), left(l), right(r), position_Y(0.0)
{
}

float Paddle::getTopCoord() const { return top; }
float Paddle::getBotCoord() const { return bot; }
float Paddle::getLeftCoord() const { return left; }
float Paddle::getRightCoord() const { return right; }

float Paddle::getPositionY() const { return position_Y; }

void Paddle::moveUp(float limit, float elapsed)
{
	if (position_Y < limit)
	{
		position_Y += 20.0 * elapsed;
		top += 20.0 * elapsed;
		bot += 20.0 * elapsed;
	}
}

void Paddle::moveDown(float limit, float elapsed)
{
	if (position_Y > limit)
	{
		position_Y -= 20.0 * elapsed;
		top -= 20.0 * elapsed;
		bot -= 20.0 * elapsed;
	}
}
