#include "stdafx.h"

#include "bullet.h"

Bullet::Bullet()
{
	position_.x = 0;
	position_.y = 0;

	direction_ = UP;
}

Bullet::Bullet(int x, int y, eDirection direction)
{
	position_.x = x;
	position_.y = y;

	direction_ = direction;
}
