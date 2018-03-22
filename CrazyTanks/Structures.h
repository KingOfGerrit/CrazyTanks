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
	ENEMY,
	VBULLET,
	HBULLET
};

enum eDirection
{
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
