#pragma once
#include "Player.h"
#include <string>
#include "Level.h"

using namespace std;

class GameSystem
{
 public:
	GameSystem(string levelFileName);

	void playGame();
	void playerMove();

 private:
	 Level _level;
	 Player _player;
};