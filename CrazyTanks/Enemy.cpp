#include "stdafx.h"

#include "Enemy.h"

Enemy::Enemy() {
	pEnemy.x = 0;
	pEnemy.y = 0;

	srand(time(NULL));

	dir = static_cast<eDirection>(rand() % 3);
}

Enemy::Enemy(int _x, int _y) {
	pEnemy.x = _x;
	pEnemy.y = _y;

	srand(time(NULL));

	dir = static_cast<eDirection>(rand() % 3);
}
