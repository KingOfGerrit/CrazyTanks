#include "stdafx.h"

#include "Player.h"

Player::Player() : Tank()
{
	life = 3;
}

Player::Player(int x, int y) : Tank(x, y)
{
	life = 3;
}
