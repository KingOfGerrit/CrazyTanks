#pragma once

#include "Structures.h"

class Player {
public:
	Player();
	Player(int _x, int _y);

	void setX(int _x) { pPlayer.x = _x; }
	void setY(int _y) { pPlayer.y = _y; }
	void setDir(eDirection _dir) { dir = _dir; }
	void decrementLife() { life--; }

	Point getPointPlayer() { return pPlayer; }
	eDirection getDir() { return dir; }
	unsigned getLife() { return life; }

private:
	Point pPlayer;
	eDirection dir;

	unsigned life;

};
