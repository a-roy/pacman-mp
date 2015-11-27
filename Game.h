//! \file
//! Game class declaration

#pragma once

#include <vector>
#include "Player.h"
#include "Field.h"

class Game
{
public:
	Field GameField;
	std::vector<Player *> Players;
	int CurrentFrame;

	Game(Field f, std::vector<Player *> p);
	Game(const Game &other);
	~Game();
	Game& operator=(const Game& rhs);
	void update();
};
