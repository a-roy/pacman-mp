#include "Game.h"
#include "Renderer.h"
#include <algorithm>

Game::Game(Field f, std::vector<Player *> p)
{
	GameField = f;
	Players = p;
	CurrentFrame = 0;
	PacmanLives = 3;
	GameOver = 0;
	Paused = 0;

	for (std::size_t i = 0; i < FIELD_WIDTH; i++)
	{
		for (std::size_t j = 0; j < FIELD_HEIGHT; j++)
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
	Paused = other.Paused;
	Players = std::vector<Player *>();
	for (std::size_t i = 0, size = other.Players.size(); i < size; i++)
	{
		Players.push_back(other.Players[i]->Clone());
	}
}

Game::~Game()
{
	for (std::size_t i = 0, size = Players.size(); i < size; i++)
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
	Paused = rhs.Paused;
	for (std::size_t i = 0, size = Players.size(); i < size; i++)
	{
		delete Players[i];
	}
	Players = std::vector<Player *>();
	for (std::size_t i = 0, size = rhs.Players.size(); i < size; i++)
	{
		Players.push_back(rhs.Players[i]->Clone());
	}
	return *this;
}

bool Game::update()
{
	if (Paused > 0)
	{
		Paused--;
		CurrentFrame++;
		return true;
	}

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
	else if (GameOver < 0)
	{
		GameOver++;
		CurrentFrame++;
		if (GameOver == 0)
		{
			return false;
		}
		return true;
	}

	Player::Event event = Player::None;
	for (std::size_t i = 0, size = Players.size(); i < size; i++)
	{
		Player *p = Players[i];
		event = (Player::Event)(event | p->Move(&GameField, Pellets));
	}
	for (unsigned int i = 0, size = Players.size(); i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++)
		{
			Position difference =
				Players[i]->CurrentPos + (Players[j]->CurrentPos * -1);
			if (i != j
					&& difference.X > -8 && difference.X < 8
					&& difference.Y > -8 && difference.Y < 8)
			{
				Player *clone_i = Players[i]->Clone();
				Player *clone_j = Players[j]->Clone();
				event = (Player::Event)(event | Players[i]->CollideWith(clone_j));
				event = (Player::Event)(event | Players[j]->CollideWith(clone_i));
				delete clone_i;
				delete clone_j;
			}
		}
	}
	if (event != Player::None)
	{
		if (event & Player::PacmanRespawned)
		{
			PacmanLives--;
		}
		if (event & Player::GhostDied)
		{
			Paused += 30;
		}
		for (std::size_t i = 0, size = Players.size(); i < size; i++)
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
		GameOver = -180;
	}
	CurrentFrame++;
	return true;
}

void Game::draw() const
{
	for (std::size_t i = 0, size = Players.size(); i < size; i++)
	{
		Players[i]->Draw();
	}
	if (GameOver > 0)
	{
		Renderer::DrawText("Pac-Man wins!", 24, 228, 444);
	}
	else if (GameOver < 0)
	{
		Renderer::DrawText("Ghosts win!", 24, 252, 444);
	}
}
