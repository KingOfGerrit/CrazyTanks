#pragma once

#include "Structures.h"

class Player
{
public:
	Player();
	Player(int _x, int _y);

	void setX(int _x) { pPlayer.x = _x; }
	void setY(int _y) { pPlayer.y = _y; }
	void setDir(eDirection _dir) { dir = _dir; }
	void setMove(eDirection _move) { move = _move; }
	void decrementLife() { life--; }

	Point getPointPlayer() { return pPlayer; }
	eDirection getDir() { return dir; }
	eDirection getMove() { return move; }
	unsigned getLife() { return life; }

private:
	Point pPlayer;
	eDirection dir;
	eDirection move;

	unsigned life;

};
