#pragma once

#include "structures.h"

class Bullet
{
public:
	Bullet();
	Bullet(int x, int y, eDirection direction);

	void setX(int x) { position_.x = x; }
	void setY(int y) { position_.y = y; }
	void setDir(eDirection direction) { direction_ = direction; }

	Point      getPosition() { return position_; }
	eDirection getDir() { return direction_; }

private:
	Point position_;
	eDirection direction_;

};
