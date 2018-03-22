#pragma once

#include "structures.h"
#include "bullet.h"

#include <vector>

class Tank
{
public:
	Tank();
	Tank(int x, int y);

	void shoot(int x, int y, eDirection direction);
	void eraseBullet(int key);

	void setX(int x)                       { position_.x = x; }
	void setY(int y)                       { position_.y = y; }
	void setDir(eDirection direction)      { direction_ = direction; }
	void setMove(eDirection move)          { move_ = move; }
	void setBullet(int key, Bullet bullet) { bullets_[key] = bullet; }

	Point      getPosition()      const { return position_; }
	eDirection getDir()           const { return direction_; }
	eDirection getMove()          const { return move_; }
	Bullet     getBullet(int key) const { return bullets_[key]; }
	size_t     getSizeBullets()   const { return bullets_.size(); }

	virtual ~Tank();

private:
	Point position_;

	eDirection direction_;
	eDirection move_;

	std::vector<Bullet> bullets_;

};
