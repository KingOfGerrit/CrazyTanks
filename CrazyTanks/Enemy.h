#pragma once

#include "Structures.h"

#include <cstdlib>
#include <ctime>

class Enemy {
public:
	Enemy();
	Enemy(int _x, int _y);

	void setX(int _x) { pEnemy.x = _x; }
	void setY(int _y) { pEnemy.y = _y; }
	void setDir(eDirection _dir) { dir = _dir; }

	Point getPointEnemy() { return pEnemy; }
	eDirection getDir() { return dir; }

private:
	Point pEnemy;
	eDirection dir;

};
