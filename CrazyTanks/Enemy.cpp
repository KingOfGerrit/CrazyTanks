#include "stdafx.h"

#include "Enemy.h"

Enemy::Enemy()
{
	pEnemy.x = 0;
	pEnemy.y = 0;

	dir = UP;
}

Enemy::Enemy(int _x, int _y)
{
	pEnemy.x = _x;
	pEnemy.y = _y;

	dir = UP;
}
