#include "stdafx.h"

#include "Game.h"

Game::Game() {
	this->Setup();
}

Game::Game(int _width, int _height) {
	this->Setup(_width, _height);
}

void Game::Setup() {
	gameOver = false;
	score = 0;
	width = 0;
	height = 0;

	player = new Player();
}

void Game::Setup(int _width, int _height) {
	this->Setup();

	width = _width;
	height = _height;

	srand(time(NULL));

	// Fill map
	for (size_t i = 0; i < height + 1; i++) {
		map.push_back(vector<int>());

		for (size_t j = 0; j < width + 1; j++) {
			// Walls
			if (i == 0 || i == height)
				map[i].push_back(1);
			else if (j == 0 || j == width)
				map[i].push_back(1);
			// Empty
			else
				map[i].push_back(0);
		}
	}

	// Spawn player
	player->setX(_width - 2);
	player->setY(_height - 2);

	map[player->getPointPlayer().y][player->getPointPlayer().x] = 2;

	// Spawn enemy
	int numberOfEnemy = rand() % 3 + 2;

	for (size_t i = 0; i < numberOfEnemy; i++) {
		bool check = false;

		while (!check) {
			int x = rand() % (width - 2) + 1, y = rand() % (height - 5) + 1;

			if (findOnMap({ x, y }, 2, 3).x == -1) {
				enemy.push_back(Enemy(x, y));
				map[y][x] = 3;
				check = true;
			}
		}
	}

	// Spawn generated walls
	int numberOfWalls = rand() % 2 + 2;

	for (size_t i = 0; i < numberOfWalls; i++) {
		bool check = false;

		while (!check) {
			Point start = { rand() % (width - 2) + 1, rand() % (height - 2) + 1 };
			Point end = { 0, 0 };

			if (map[start.y][start.x] != 0)
				break;
			if (findOnMap({ start.x, start.y }, 1, 2).x != -1 && findOnMap({ start.x, start.y }, 1, 3).x != -1)
				break;
			if (findOnMap({ start.x, start.y }, 1, 1).x != -1)
				break;

			if (rand() % 2)
				end = { start.x, rand() % (height - 2) + 1 };
			else
				end = { rand() % (width - 2) + 1, start.y };

			if (map[end.y][end.x] != 0)
				break;
			if (findOnMap({ end.x, end.y }, 1, 2).x != -1 && findOnMap({ end.x, end.y }, 1, 3).x != -1)
				break;
			if (findOnMap({ end.x, end.y }, 1, 1).x != -1)
				break;

			drawLine(start, end);

			check = true;
		}
	}

}

void Game::Draw() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");

	srand(time(NULL));

	//0 = Black		8 = Gray
	//1 = Blue		9 = Light Blue
	//2 = Green		a = Light Green
	//3 = Aqua		b = Light Aqua
	//4 = Red		c = Light Red
	//5 = Purple	d = Light Purple
	//6 = Yellow	e = Light Yellow
	//7 = White		f = Bright White
	for (size_t i = 0; i < map.size(); i++) {
		for (size_t j = 0; j < map[i].size(); j++) {
			switch (map[i][j])
			{
			case 0:
				cout << ' ';
				break;
			case 1:
				SetConsoleTextAttribute(hConsole, 15);
				cout << '#';
				break;
			case 2:
				SetConsoleTextAttribute(hConsole, 2);

				switch (player->getDir())
				{
				case LEFT:
					// ╣ <-code: 185
					cout << (char)185;
					break;
				case RIGHT:
					// ╠ <-code: 204
					cout << (char)204;
					break;
				case UP:
					// ╩ <-code: 202 
					cout << (char)202;
					break;
				case DOWN:
					// ╦ <-code: 203
					cout << (char)203;
					break;
				}
				
				break;
			case 3:
				SetConsoleTextAttribute(hConsole, 4);
				cout << 'E';
				break;
			}
		}
		
		cout << endl;
	}

	SetConsoleTextAttribute(hConsole, 7);
	cout << "Score: " << score << endl;
	cout << "Life: " << player->getLife() << endl;
	cout << "Time: " << clock() / 1000.0 << endl;
}

void Game::Logic() {

}

void Game::drawLine(Point p1, Point p2) {
	const int deltaX = abs(p2.x - p1.x);
	const int deltaY = abs(p2.y - p1.y);
	const int signX = p1.x < p2.x ? 1 : -1;
	const int signY = p1.y < p2.y ? 1 : -1;
	
	int error = deltaX - deltaY;

	map[p2.y][p2.x] = 1;
	while (p1.x != p2.x || p1.y != p2.y) {
		map[p1.y][p1.x] = 1;
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

Point Game::findOnMap(Point center, int radius, int i) {
	Point p = { center.x - radius, center.y - radius };
	Point f = { -1, -1 };

	if (p.x < 1)
		p.x = 1;
	if (p.y < 1)
		p.y = 1;

	for (; p.y < center.y + radius; p.y++) {
		for (int j = p.x; j < center.x + radius; j++) {
			if (map[p.y][j] == i)
				return f = { j, p.y };
		}
	}

	return f;
}
