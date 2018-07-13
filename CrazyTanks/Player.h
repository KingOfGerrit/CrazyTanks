#pragma once

//#include "structures.h"
#include "tank.h" 

class Player : public Tank
{
public:
	Player();
	Player(int x, int y);

	void decrementLife() { life--; }

	unsigned getLife() const { return life; }

private:
	unsigned life;

};
