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
		Field::PelletStatus AllPellets;
		Field::PelletStatus Pellets;
		std::vector<Player *> Players;
		unsigned int CurrentFrame;
		unsigned int PacmanLives;
	
		Game(Field f, std::vector<Player *> p);
		Game(const Game &other);
		~Game();
		Game& operator=(const Game& rhs);
		bool update();
		void draw() const;
	private:
		int GameOver;
		int Paused;
};
