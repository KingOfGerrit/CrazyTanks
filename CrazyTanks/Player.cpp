#include "stdafx.h"

#include "Player.h"

Player::Player() {
	pPlayer.x = 0;
	pPlayer.y = 0;

	dir = UP;

	life = 3;
}

Player::Player(int _x, int _y) {
	pPlayer.x = _x;
	pPlayer.y = _y;

	dir = UP;

	life = 3;
}
