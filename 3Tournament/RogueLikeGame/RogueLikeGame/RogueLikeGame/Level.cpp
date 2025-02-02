#include "Level.h"
#include <fstream>
#include <iostream>
#include <cstdio>

Level::Level() {

}

void Level::load(string fileName, Player& player) {

	ifstream file;

	file.open(fileName);
	if (file.fail()) {
		perror(fileName.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;

	while (getline(file, line)) {
		_levelData.push_back(line);
	}

	file.close();

	//
	char tile;

	for (int i = 0; i < _levelData.size(); i++) {
		for (int j = 0; j < _levelData[i].size(); j++) {
			tile = _levelData[i][j];

			switch (tile) {
			case '@':
				player.setPosition(j, i);
				break;
			case 'Z': 
				_enemies.push_back(Enemy("Zombie", tile, 1, 3, 1, 10, 10));
				_enemies.back().setPosition(j, i);
				break;
			case 'D':
				_enemies.push_back(Enemy("Dragon", tile, 2, 55, 25, 30, 100));
				_enemies.back().setPosition(j, i);
				break;
			}
		}
	}
}

void Level::print() {

	cout << string(100, '\n');

	for (int i = 0; i < _levelData.size(); i++) {
		printf("%s\n", _levelData[i].c_str());
	}
	printf("\n");
}

void Level::movePlayer(char input, Player &player) {

	int playerX;
	int playerY;

	player.getPosition(playerX, playerY);

	char moveTile;

	switch (input) {

		case 'w':
		case 'W':
			processPlayerMove(player, playerX, playerY - 1);
			break;

		case 's':
		case 'S':
			processPlayerMove(player, playerX, playerY + 1);
			break;

		case 'a':
		case 'A':
			processPlayerMove(player, playerX - 1, playerY);
			break;

		case 'd':
		case 'D':
			processPlayerMove(player, playerX + 1, playerY);
			break;

		default:
			printf("Invalid input!\n");
			system("PAUSE");
			break;
	}
}

char Level::getTile(int x, int y) {
	return _levelData[y][x];
}

void Level::setTile(int x, int y, char tile) {
	_levelData[y][x] = tile;
}

void Level::processPlayerMove(Player& player, int targetX, int targetY) {
	
	int playerX;
	int playerY;
	player.getPosition(playerX, playerY);
	
	char moveTile = getTile(targetX, targetY);

	switch (moveTile)
	{
	case '.':
		player.setPosition(targetX, targetY);
		setTile(playerX, playerY, '.');
		setTile(targetX, targetY, '@');
		break;
	case '#':
		break;
	default:
		battleMonster(player, targetX, targetY);
		break;
	}
}

void Level::battleMonster(Player& player, int targetX, int targetY) {

	int enemyX;
	int enemyY;
	int attackRoll;
	int attackResult;

	for (int i = 0; i < _enemies.size(); i++) {

		_enemies[i].getPosition(enemyX, enemyY);

		if (targetX == enemyX && targetY == enemyY) {

			attackRoll = player.attack();

			printf("You attacked monster with a roll of %d\n", attackRoll);

			attackResult = _enemies[i].takeDamage(attackRoll);
			if (attackResult != 0) {
				setTile(targetX, targetY, '.');
				print();
				printf("Monster died!\n");
				system("PAUSE");
				player.addExperience(attackResult);
				return;
			}
			
			attackRoll = _enemies[i].attack();
			attackResult = player.takeDamage(attackRoll);

			if (attackResult != 0) {
				print();
				setTile(targetX, targetY, 'x');
				printf("You died!\n");
				system("PAUSE");
				exit(0);
			}
			system("PAUSE");
			return;
		}
	}
}