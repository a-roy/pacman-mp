#include "Game.h"

Game::Game(Field f, std::vector<Player *> p)
{
	GameField = f;
	Players = p;
	CurrentFrame = 0;
}

Game::Game(const Game &other)
{
	GameField = other.GameField;
	Pellets = other.Pellets;
	CurrentFrame = other.CurrentFrame;
	Players = std::vector<Player *>();
	for (unsigned int i = 0, size = other.Players.size(); i < size; i++)
	{
		Players.push_back(other.Players[i]->Clone());
	}
}

Game::~Game()
{
	for (unsigned int i = 0, size = Players.size(); i < size; i++)
	{
		delete Players[i];
	}
}

Game& Game::operator=(const Game& rhs)
{
	GameField = rhs.GameField;
	Pellets = rhs.Pellets;
	CurrentFrame = rhs.CurrentFrame;
	for (unsigned int i = 0, size = Players.size(); i < size; i++)
	{
		delete Players[i];
	}
	Players = std::vector<Player *>();
	for (unsigned int i = 0, size = rhs.Players.size(); i < size; i++)
	{
		Players.push_back(rhs.Players[i]->Clone());
	}
	return *this;
}

void Game::update()
{
	for (int i = 0; i < Players.size(); i++)
	{
		Player *p = Players[i];
		p->Move(&GameField, Pellets);
		p->AnimFrame++;
	}
	CurrentFrame++;
}
