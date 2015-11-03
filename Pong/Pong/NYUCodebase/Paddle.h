#pragma once

class Paddle
{
public:
	Paddle(float t, float b, float l, float r);

	float getTopCoord() const;
	float getBotCoord() const;
	float getLeftCoord() const;
	float getRightCoord() const;

	float getPositionY() const;

	void moveUp(float limit, float elapsed);
	void moveDown(float limit, float elapsed);

private:
	float top;
	float bot;
	const float left;
	const float right;

	float position_Y;
};

