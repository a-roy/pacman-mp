#include "Game.h"
#include <algorithm>

Game::Game(Field f, std::vector<Player *> p)
{
	GameField = f;
	Players = p;
	CurrentFrame = 0;
	PacmanLives = 3;
	GameOver = 0;

	for (unsigned int i = 0; i < FIELD_WIDTH; i++)
	{
		for (unsigned int j = 0; j < FIELD_HEIGHT; j++)
		{
			if ((f.Tiles[i][j] & Field::Pellet) == Field::Pellet)
			{
				AllPellets.Eat(i, j);
			}
		}
	}
}

Game::Game(const Game &other)
{
	GameField = other.GameField;
	AllPellets = other.AllPellets;
	Pellets = other.Pellets;
	CurrentFrame = other.CurrentFrame;
	PacmanLives = other.PacmanLives;
	GameOver = other.GameOver;
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
	AllPellets = rhs.AllPellets;
	Pellets = rhs.Pellets;
	CurrentFrame = rhs.CurrentFrame;
	PacmanLives = rhs.PacmanLives;
	GameOver = rhs.GameOver;
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

bool Game::update()
{
	if (GameOver > 0)
	{
		GameOver--;
		CurrentFrame++;
		if (GameOver == 0)
		{
			return false;
		}
		return true;
	}

	Player::Event event = Player::None;
	for (unsigned int i = 0; i < Players.size(); i++)
	{
		Player *p = Players[i];
		Player::Event e = p->Move(&GameField, Pellets);
		if (e > event)
		{
			event = e;
		}
	}
	for (unsigned int i = 0; i < Players.size(); i++)
	{
		for (unsigned int j = 0; j < Players.size(); j++)
		{
			if (i != j
					&& Players[i]->XPos - Players[j]->XPos < TILE_SIZE / 2
					&& Players[j]->XPos - Players[i]->XPos < TILE_SIZE / 2
					&& Players[i]->YPos - Players[j]->YPos < TILE_SIZE / 2
					&& Players[j]->YPos - Players[i]->YPos < TILE_SIZE / 2)
			{
				Player::Event e;
				Player *clone_i = Players[i]->Clone();
				Player *clone_j = Players[j]->Clone();
				e = Players[i]->CollideWith(clone_j);
				if (e > event)
				{
					event = e;
				}
				e = Players[j]->CollideWith(clone_i);
				if (e > event)
				{
					event = e;
				}
				delete clone_i;
				delete clone_j;
			}
		}
	}
	if (event != Player::None)
	{
		if (event == Player::PacmanRespawned)
		{
			PacmanLives--;
		}
		for (unsigned int i = 0; i < Players.size(); i++)
		{
			Players[i]->ProcessEvent(event);
		}
	}

	if (Pellets == AllPellets)
	{
		GameOver = 180;
	}
	else if (PacmanLives == 0)
	{
		GameOver = 180;
	}
	CurrentFrame++;
	return true;
}

void Game::draw() const
{
	for (unsigned int i = 0; i < Players.size(); i++)
	{
		Players[i]->Draw();
	}
}
