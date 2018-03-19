#pragma once

struct Point
{
	int x;
	int y;
};

enum eTags
{
	EMPTY = 0,
	WALL,
	PLAYER,
	ENEMY
};

enum eDirection
{
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
