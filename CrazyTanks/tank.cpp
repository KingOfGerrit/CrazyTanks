#include "stdafx.h"

#include "tank.h"

Tank::Tank()
{
	position_.x = 0;
	position_.y = 0;

	direction_ = UP;
	move_      = STOP;
}

Tank::Tank(int x, int y)
{
	position_.x = x;
	position_.y = y;

	direction_ = UP;
	move_      = STOP;
}

void Tank::shoot(int x, int y, eDirection direction)
{
	if (bullets_.size() < 1)
		bullets_.push_back(Bullet(x, y, direction));
}

void Tank::eraseBullet(int key)
{
	bullets_.erase(bullets_.begin() + key);
}

Tank::~Tank()
{
	position_.x = -1;
	position_.y = -1;

	direction_ = STOP;
	move_      = STOP;
}
