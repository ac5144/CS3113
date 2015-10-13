#pragma once
class Projectile
{
public:
	Projectile(float x, float y, float v);
	~Projectile();

	float getPositionX() const;
	float getPositionY() const;

	void move_x(float x);
	void move_y(float y);

	float getVelocityY() const;

private:
	float position_x;
	float position_y;

	float velocity_y;
};

