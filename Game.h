//! \file
//! Game class declaration

#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include "Player.h"
#include "Field.h"

class Game
{
public:
	Field GameField;
	Field::PelletStatus Pellets;
	std::vector<Player *> Players;
	unsigned int CurrentFrame;

	Game(Field f, std::vector<Player *> p);
	Game(const Game &other);
	~Game();
	Game& operator=(const Game& rhs);
	void update();
};
