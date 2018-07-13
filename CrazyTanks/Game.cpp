#include "stdafx.h"

#include "game.h"

Game::Game()
{
	this->Setup();
}

Game::Game(int width, int height)
{
	this->Setup(width, height);
}

void Game::Setup()
{
	gameOver_ = false;
	score_ = 0;
	width_ = 0;
	height_ = 0;

	player_ = new Player();
}

void Game::Setup(int width, int height)
{
	this->Setup();

	width_  = width;
	height_ = height;
	map_    = vector<vector<int>>(height_ + 1, vector<int>(width_ + 1, 0));

	// Fill border
	for (size_t i = 0; i < height_ + 1; i++) {
		for (size_t j = 0; j < width_ + 1; j++) {
			if      (i == 0 || i == height_) map_[i][j] = WALL;
			else if (j == 0 || j == width_)  map_[i][j] = WALL;
			else                             map_[i][j] = EMPTY;
		}
	}

	// Spawn player
	player_->setX(width_ - 2);
	player_->setY(height_ - 2);

	map_[height_ - 2][width_ - 2] = PLAYER;

	// Spawn enemy
	int numberOfEnemy = getRandomNumber(5, 7);

	spawnEnemy(numberOfEnemy);

	// Spawn walls
	int numberOfWalls = getRandomNumber(3, 5);

	spawnWalls(numberOfWalls);
}

void Game::Draw()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");

	// ╣ <-code: 185
	// ╠ <-code: 204
	// ╩ <-code: 202
	// ╦ <-code: 203

	//0 = Black		8 = Gray
	//1 = Blue		9 = Light Blue
	//2 = Green		a = Light Green
	//3 = Aqua		b = Light Aqua
	//4 = Red		c = Light Red
	//5 = Purple	d = Light Purple
	//6 = Yellow	e = Light Yellow
	//7 = White		f = Bright White
	for (size_t i = 0; i < map_.size(); i++) {
		for (size_t j = 0; j < map_[i].size(); j++) {
			switch (map_[i][j]) {
				case EMPTY:
					cout << ' ';
					break;

				case WALL:
					SetConsoleTextAttribute(hConsole, 15);
					cout << '#';
					break;

				case PLAYER:
					SetConsoleTextAttribute(hConsole, 2);

					switch (player_->getDir()) {
						case LEFT: cout << (char)185; break;
						case RIGHT: cout << (char)204; break;
						case UP: cout << (char)202; break;
						case DOWN: cout << (char)203; break;
					}
					break;

				case ENEMY:
					SetConsoleTextAttribute(hConsole, 4);

					switch (getEnemyByCoordinates({ static_cast<int>(j), static_cast<int>(i) }).getDir()) {
						case LEFT : cout << (char)185; break;
						case RIGHT: cout << (char)204; break;
						case UP   : cout << (char)202; break;
						case DOWN : cout << (char)203; break;
					}
					break;

				case VBULLET:
					SetConsoleTextAttribute(hConsole, 6);
					cout << '-';
					break;

				case HBULLET:
					SetConsoleTextAttribute(hConsole, 6);
					cout << '|';
					break;
			}
		}
		
		cout << endl;
	}

	SetConsoleTextAttribute(hConsole, 7);
	cout << "Score: " << score_ << endl;
	cout << "Life: " << player_->getLife() << endl;
	cout << "Time: " << clock() / 1000.0 << endl;
}

void Game::Input()
{
	if (_kbhit()) {
		// LEFT : -32 75
		// RIGHT: -32 77
		// UP   : -32 72
		// DOWN : -32 80
		// SPACE:  32

		char c = _getch();
		switch(c) {
			case -32:
				c = _getch();
				switch (c) {
					case 75:
						player_->setDir(LEFT);
						player_->setMove(LEFT);
						break;

					case 77:
						player_->setDir(RIGHT);
						player_->setMove(RIGHT);
						break;

					case 72:
						player_->setDir(UP);
						player_->setMove(UP);
						break;

					case 80:
						player_->setDir(DOWN);
						player_->setMove(DOWN);
						break;
				}
				break;

			case 32:
				Point p = player_->getPosition();
				player_->shoot(p.x, p.y, player_->getDir());
				break;

			case 'x':
				gameOver_ = true;
				break;
		}
	}
	else {
		player_->setMove(STOP);
	}
}

void Game::Logic()
{
	// Player logic
	Point p = player_->getPosition();

	if (player_->getMove() != STOP) {
		switch (player_->getMove()) {
			case LEFT:
				if (map_[p.y][p.x - 1] == EMPTY) {
					player_->setX(p.x - 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y][p.x - 1] = PLAYER;
				}
				break;

			case RIGHT:
				if (map_[p.y][p.x + 1] == EMPTY) {
					player_->setX(p.x + 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y][p.x + 1] = PLAYER;
				}
				break;

			case UP:
				if (map_[p.y - 1][p.x] == EMPTY) {
					player_->setY(p.y - 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y - 1][p.x] = PLAYER;
				}
				break;

			case DOWN:
				if (map_[p.y + 1][p.x] == EMPTY) {
					player_->setY(p.y + 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y + 1][p.x] = PLAYER;
				}
				break;
		}
	}

	// Player bullets
	size_t size = player_->getSizeBullets();
	if (size > 0) {
		for (size_t i = 0; i < size; i++) {
			Bullet bullet = player_->getBullet(i);
			Point p = bullet.getPosition();
			bool isBullet = false;
			switch (bullet.getDir()) {
				case LEFT:
					isBullet = map_[p.y][p.x - 1] == HBULLET || map_[p.y][p.x - 1] == VBULLET;
					if (map_[p.y][p.x - 1] == EMPTY || isBullet) {
						bullet.setX(p.x - 1);
						player_->setBullet(i, bullet);

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y][p.x - 1] = VBULLET;
					}
					else if (map_[p.y][p.x - 1] == ENEMY) {
						player_->eraseBullet(i);
						eraseEnemyByCoordinates({ p.x - 1, p.y });
						score_ += 10;

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y][p.x - 1] = EMPTY;
					}
					else if (map_[p.y][p.x - 1] == WALL) {
						player_->eraseBullet(i);
						map_[p.y][p.x] = EMPTY;
					}
					break;

				case RIGHT:
					isBullet = map_[p.y][p.x + 1] == HBULLET || map_[p.y][p.x + 1] == VBULLET;
					if (map_[p.y][p.x + 1] == EMPTY || isBullet) {
						bullet.setX(p.x + 1);
						player_->setBullet(i, bullet);

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y][p.x + 1] = VBULLET;
					}
					else if (map_[p.y][p.x + 1] == ENEMY) {
						player_->eraseBullet(i);
						eraseEnemyByCoordinates({ p.x + 1, p.y });
						score_ += 10;

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y][p.x + 1] = EMPTY;
					}
					else if (map_[p.y][p.x + 1] == WALL) {
						player_->eraseBullet(i);
						map_[p.y][p.x] = EMPTY;
					}
					break;

				case UP:
					isBullet = map_[p.y - 1][p.x] == HBULLET || map_[p.y - 1][p.x] == VBULLET;
					if (map_[p.y - 1][p.x] == EMPTY || isBullet) {
						bullet.setY(p.y - 1);
						player_->setBullet(i, bullet);

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y - 1][p.x] = HBULLET;
					}
					else if (map_[p.y - 1][p.x] == ENEMY) {
						player_->eraseBullet(i);
						eraseEnemyByCoordinates({ p.x, p.y - 1 });
						score_ += 10;

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y - 1][p.x] = EMPTY;
					}
					else if (map_[p.y - 1][p.x] == WALL) {
						player_->eraseBullet(i);
						map_[p.y][p.x] = EMPTY;
					}
					break;

				case DOWN:
					isBullet = map_[p.y + 1][p.x] == HBULLET || map_[p.y + 1][p.x] == VBULLET;
					if (map_[p.y + 1][p.x] == EMPTY || isBullet) {
						bullet.setY(p.y + 1);
						player_->setBullet(i, bullet);

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y + 1][p.x] = HBULLET;
					}
					else if (map_[p.y + 1][p.x] == ENEMY) {
						player_->eraseBullet(i);
						eraseEnemyByCoordinates({ p.x, p.y + 1 });
						score_ += 10;

						if (map_[p.y][p.x] != PLAYER) map_[p.y][p.x] = EMPTY;
						map_[p.y + 1][p.x] = EMPTY;
					}
					else if (map_[p.y + 1][p.x] == WALL) {
						player_->eraseBullet(i);
						map_[p.y][p.x] = EMPTY;
					}
					break;
			}
		}
	}

	// Enemy logic
	for (size_t i = 0; i < enemy_.size(); i++) {
		p = enemy_[i].getPosition();
		// Find player
		if (getRandomNumber(1, 10) > 9) {
			bool isPlayer = false;
			// Check left
			for (size_t j = p.x - 1; map_[p.y][j] != WALL; j--) {
				if (map_[p.y][j] == PLAYER) {
					if (enemy_[i].getDir() == LEFT) {
						enemy_[i].shoot(p.x, p.y, LEFT);
						isPlayer = true;
						break;
					}
					else {
						enemy_[i].setDir(LEFT);
						enemy_[i].setMove(LEFT);
						isPlayer = true;
						break;
					}
				}
			}

			if (isPlayer) continue;

			// Check right
			for (size_t j = p.x + 1; map_[p.y][j] != WALL; j++) {
				if (map_[p.y][j] == PLAYER) {
					if (enemy_[i].getDir() == RIGHT) {
						enemy_[i].shoot(p.x, p.y, RIGHT);
						isPlayer = true;
						break;
					}
					else {
						enemy_[i].setDir(RIGHT);
						enemy_[i].setMove(RIGHT);
						isPlayer = true;
						break;
					}
				}
			}

			if (isPlayer) continue;

			// Check up
			for (size_t j = p.y - 1; map_[j][p.x] != WALL; j--) {
				if (map_[j][p.x] == PLAYER) {
					if (enemy_[i].getDir() == UP) {
						enemy_[i].shoot(p.x, p.y, UP);
						isPlayer = true;
						break;
					}
					else {
						enemy_[i].setDir(UP);
						enemy_[i].setMove(UP);
						isPlayer = true;
						break;
					}
				}
			}

			if (isPlayer) continue;

			// Check down
			for (size_t j = p.y + 1; map_[j][p.x] != WALL; j++) {
				if (map_[j][p.x] == PLAYER) {
					if (enemy_[i].getDir() == DOWN) {
						enemy_[i].shoot(p.x, p.y, DOWN);
						isPlayer = true;
						break;
					}
					else {
						enemy_[i].setDir(DOWN);
						enemy_[i].setMove(DOWN);
						isPlayer = true;
						break;
					}
				}
			}

			if (!isPlayer) {
				if (enemy_[i].getMove() == STOP) {
					eDirection newDirection = static_cast<eDirection>(getRandomNumber(1, 4));
					enemy_[i].setDir(newDirection);
					enemy_[i].setMove(newDirection);
				}
				else {
					// 20%
					if (getRandomNumber(1, 10) > 8) {
						eDirection newDirection = static_cast<eDirection>(getRandomNumber(1, 4));
						enemy_[i].setDir(newDirection);
						enemy_[i].setMove(newDirection);
					}
				}
			}
		}
		else {
			enemy_[i].setMove(STOP);
		}

		// Move enemy
		if (enemy_[i].getMove() != STOP) {
			switch (enemy_[i].getMove()) {
			case LEFT:
				if (map_[p.y][p.x - 1] == EMPTY) {
					enemy_[i].setX(p.x - 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y][p.x - 1] = ENEMY;
				}
				break;

			case RIGHT:
				if (map_[p.y][p.x + 1] == EMPTY) {
					enemy_[i].setX(p.x + 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y][p.x + 1] = ENEMY;
				}
				break;

			case UP:
				if (map_[p.y - 1][p.x] == EMPTY) {
					enemy_[i].setY(p.y - 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y - 1][p.x] = ENEMY;
				}
				break;

			case DOWN:
				if (map_[p.y + 1][p.x] == EMPTY) {
					enemy_[i].setY(p.y + 1);
					map_[p.y][p.x] = EMPTY;
					map_[p.y + 1][p.x] = ENEMY;
				}
				break;
			}
		}

		size_t size = enemy_[i].getSizeBullets();
		if (size > 0) {
			for (size_t j = 0; j < size; j++) {
				Bullet bullet = enemy_[i].getBullet(j);
				Point p = bullet.getPosition();
				bool isBullet = false;
				switch (bullet.getDir()) {
				case LEFT:
					isBullet = map_[p.y][p.x - 1] == HBULLET || map_[p.y][p.x - 1] == VBULLET;
					if (map_[p.y][p.x - 1] == EMPTY || isBullet) {
						bullet.setX(p.x - 1);
						enemy_[i].setBullet(j, bullet);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						map_[p.y][p.x - 1] = VBULLET;
					}
					else if (map_[p.y][p.x - 1] == PLAYER) {
						if (player_->getLife() > 1) {
							player_->decrementLife();
						}
						else {
							gameOver_ = true;
						}
						
						enemy_[i].eraseBullet(j);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						//map_[p.y][p.x - 1] = EMPTY;
					}
					else if (map_[p.y][p.x - 1] == WALL) {
						enemy_[i].eraseBullet(j);
						map_[p.y][p.x] = EMPTY;
					}
					break;

				case RIGHT:
					isBullet = map_[p.y][p.x + 1] == HBULLET || map_[p.y][p.x + 1] == VBULLET;
					if (map_[p.y][p.x + 1] == EMPTY || isBullet) {
						bullet.setX(p.x + 1);
						enemy_[i].setBullet(j, bullet);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						map_[p.y][p.x + 1] = VBULLET;
					}
					else if (map_[p.y][p.x + 1] == PLAYER) {
						if (player_->getLife() > 1) {
							player_->decrementLife();
						}
						else {
							gameOver_ = true;
						}

						enemy_[i].eraseBullet(j);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						//map_[p.y][p.x + 1] = EMPTY;
					}
					else if (map_[p.y][p.x + 1] == WALL) {
						enemy_[i].eraseBullet(j);
						map_[p.y][p.x] = EMPTY;
					}
					break;

				case UP:
					isBullet = map_[p.y - 1][p.x] == HBULLET || map_[p.y - 1][p.x] == VBULLET;
					if (map_[p.y - 1][p.x] == EMPTY || isBullet) {
						bullet.setY(p.y - 1);
						enemy_[i].setBullet(j, bullet);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						map_[p.y - 1][p.x] = HBULLET;
					}
					else if (map_[p.y - 1][p.x] == PLAYER) {
						if (player_->getLife() > 1) {
							player_->decrementLife();
						}
						else {
							gameOver_ = true;
						}

						enemy_[i].eraseBullet(j);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						//map_[p.y - 1][p.x] = EMPTY;
					}
					else if (map_[p.y - 1][p.x] == WALL) {
						enemy_[i].eraseBullet(j);
						map_[p.y][p.x] = EMPTY;
					}
					break;

				case DOWN:
					isBullet = map_[p.y + 1][p.x] == HBULLET || map_[p.y + 1][p.x] == VBULLET;
					if (map_[p.y + 1][p.x] == EMPTY || isBullet) {
						bullet.setY(p.y + 1);
						enemy_[i].setBullet(j, bullet);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						map_[p.y + 1][p.x] = HBULLET;
					}
					else if (map_[p.y + 1][p.x] == PLAYER) {
						if (player_->getLife() > 1) {
							player_->decrementLife();
						}
						else {
							gameOver_ = true;
						}

						enemy_[i].eraseBullet(j);

						if (map_[p.y][p.x] != ENEMY) map_[p.y][p.x] = EMPTY;
						//map_[p.y + 1][p.x] = EMPTY;
					}
					else if (map_[p.y + 1][p.x] == WALL) {
						enemy_[i].eraseBullet(j);
						map_[p.y][p.x] = EMPTY;
					}
					break;
				}
			}
		}
	}
}

void Game::drawLine(Point p1, Point p2)
{
	const int deltaX = abs(p2.x - p1.x);
	const int deltaY = abs(p2.y - p1.y);
	const int signX = p1.x < p2.x ? 1 : -1;
	const int signY = p1.y < p2.y ? 1 : -1;
	
	int error = deltaX - deltaY;

	map_[p2.y][p2.x] = WALL;
	while (p1.x != p2.x || p1.y != p2.y) {
		map_[p1.y][p1.x] = WALL;
		const int error2 = error * 2;

		if (error2 > -deltaY) {
			error -= deltaY;
			p1.x += signX;
		}
		if (error2 < deltaX) {
			error += deltaX;
			p1.y += signY;
		}
	}
}

void Game::spawnEnemy(int numberOfEnemy)
{
	for (size_t i = 0; i < numberOfEnemy; i++) {
		bool check = false;
		while (!check) {
			int x = getRandomNumber(1, width_ - 2);
			int	y = getRandomNumber(1, height_ - 5);

			if (findOnMap({ x, y }, 2, ENEMY).x == -1) {
				enemy_.push_back(Enemy(x, y));
				map_[y][x] = ENEMY;
				check = true;
			}
		}
	}
}

void Game::spawnWalls(int numberOfWalls)
{
	for (size_t i = 0; i < numberOfWalls; i++) {
		bool check = false;
		while (!check) {
			Point start = { getRandomNumber(1, width_ - 2), getRandomNumber(1, height_ - 2) };
			Point end   = { 0, 0 };

			bool isPlayerNearby = findOnMap({ start.x, start.y }, 1, PLAYER).x != -1;
			bool isEnemyNearby  = findOnMap({ start.x, start.y }, 1, ENEMY).x  != -1;
			bool isWallNearby   = findOnMap({ start.x, start.y }, 1, WALL).x   != -1;
			if (map_[start.y][start.x] != EMPTY) break;
			if (isPlayerNearby && isEnemyNearby) break;
			if (isWallNearby)                    break;

			if (getRandomNumber(0, 1)) end = { start.x, getRandomNumber(1, height_ - 2) };
			else                       end = { getRandomNumber(1, width_ - 2), start.y };

			isPlayerNearby = findOnMap({ end.x, end.y }, 1, PLAYER).x != -1;
			isEnemyNearby  = findOnMap({ end.x, end.y }, 1, ENEMY).x  != -1;
			isWallNearby   = findOnMap({ end.x, end.y }, 1, WALL).x   != -1;
			if (map_[end.y][end.x] != EMPTY)     break;
			if (isPlayerNearby && isEnemyNearby) break;
			if (isWallNearby)                    break;

			drawLine(start, end);

			check = true;
		}
	}
}

void Game::eraseEnemyByCoordinates(Point p)
{
	for (auto it = enemy_.begin(); it != enemy_.end(); it++) {
		bool isEnemyOnPoint = it->getPosition().x == p.x && it->getPosition().y == p.y;
		if (isEnemyOnPoint) {
			for (size_t i = 0; i < it->getSizeBullets(); i++) {
				Point p = it->getBullet(i).getPosition();
				map_[p.y][p.x] = EMPTY;
				it->eraseBullet(i);
			}
			enemy_.erase(it);
			return;
		}
	}
}

Point Game::findOnMap(Point center, int radius, int i)
{
	Point p = { center.x - radius, center.y - radius };
	Point f = { -1, -1 };

	if (p.x < 1) p.x = 1;
	if (p.y < 1) p.y = 1;

	for (; p.y < center.y + radius; p.y++) {
		for (int j = p.x; j < center.x + radius; j++) {
			if (map_[p.y][j] == i) return f = { j, p.y };
		}
	}	

	return f;
}

Enemy Game::getEnemyByCoordinates(Point p)
{
	for (size_t i = 0; i < enemy_.size(); i++) {
		bool isEnemyOnPoint = enemy_[i].getPosition().x == p.x && enemy_[i].getPosition().y == p.y;
		if (isEnemyOnPoint) return enemy_[i];
	}

	return Enemy(-1, -1);
}

int Game::getRandomNumber(int start, int stop)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(start, stop);

	return dist(mt);
}

Game::~Game ()
{
	delete player_;

	for (size_t i = 0; i < map_.size(); i++)
		map_[i].clear();

	map_.clear();
	enemy_.clear();
}
