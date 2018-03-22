#pragma once

//#include "structures.h"
#include "tank.h"

class Enemy : public Tank
{
public:
	Enemy()             : Tank()     {}
	Enemy(int x, int y) : Tank(x, y) {}

};
