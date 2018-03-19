#pragma once

#include <iostream>
#include <conio.h>
#include <vector>
#include <string> 
#include <ctime>
#include <cstdlib>
#include <Windows.h>

#include "structures.h"

#include "player.h"
#include "enemy.h"

using namespace std;

//TODO: разобраться с очисткой памяти и указателями
class Game
{
public:
	Game();
	Game(int _width, int _height);

	void Setup();
	void Setup(int _width, int _height);
	void Draw();
	void Input();
	void Logic();
	
	void drawLine(Point p1, Point p2);
	void spawnEnemy(int numberOfEnemy);
	void spawnWalls(int numberOfWalls);
	Point findOnMap(Point center, int radius, int i);
	Enemy getEnemyByCoordinates(Point p);

	void setGameOver(bool _gameOver) { gameOver = _gameOver; }
	void addScore(int addendum) { score += addendum; }
	void setScore(int _score) { score = _score; }
	void setWidth(int _width) { width = _width; }
	void setHeight(int _height) { height = _height; }

	bool getGameOver() { return gameOver; }
	int getScore() { return score; }
	int getWidth() { return width; }
	int getHeight() { return height; }

	~Game();

private:
	bool gameOver;
	int score;
	int width;
	int height;
	
	// 0 - empty, 1 - wall, 2 - player, 3 - enemy
	vector<vector<int>> map;

	Player *player;
	vector<Enemy> enemy;

};
