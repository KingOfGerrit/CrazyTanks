#pragma once

#include <iostream>
#include <conio.h>
#include <Windows.h>

#include <vector>
#include <string> 
#include <ctime>
#include <cstdlib>
#include <random>
#include <exception>

#include "structures.h"

#include "player.h"
#include "enemy.h"
#include "bullet.h"

using namespace std;

//TODO: разобраться с очисткой памяти и указателями
//gameOver_ переименовать в isGameOver_
class Game
{
public:
	Game();
	Game(int width, int height);

	void Setup();
	void Setup(int width, int height);
	void Draw();
	void Input();
	void Logic();
	
	void  drawLine(Point p1, Point p2);
	void  spawnEnemy(int numberOfEnemy);
	void  spawnWalls(int numberOfWalls);
	void  eraseEnemyByCoordinates(Point p);
	Point findOnMap(Point center, int radius, int i);
	Enemy getEnemyByCoordinates(Point p);
	int   getRandomNumber(int start, int stop);

	void setGameOver(bool gameOver) { gameOver_ = gameOver; }
	void addScore(int addendum)     { score_ += addendum; }
	void setScore(int score)        { score_ = score; }
	void setWidth(int width)        { width_ = width; }
	void setHeight(int height)      { height_ = height; }

	bool getGameOver() const { return gameOver_; }
	int  getScore()    const { return score_; }
	int  getWidth()    const { return width_; }
	int  getHeight()   const { return height_; }

	~Game();

private:
	bool gameOver_;
	int  score_;
	int  width_;
	int  height_;

	vector<vector<int>> map_;

	Player*       player_;
	vector<Enemy> enemy_;

};
