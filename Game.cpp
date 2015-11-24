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
	CurrentFrame = other.CurrentFrame;
	Players = std::vector<Player *>();
	for (unsigned int i = 0, size = Players.size(); i < size; i++)
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
		if (move(p, p->NextDir))
		{
			p->CurrentDir = p->NextDir;
		}
		else
		{
			move(p, p->CurrentDir);
		}
		p->AnimFrame++;
	}
	CurrentFrame++;
}

bool Game::move(Player *p, Player::Direction d)
{
	int dx, dy;
	switch (d)
	{
		case Player::Right:
			dx = 1;
			dy = 0;
			break;
		case Player::Up:
			dx = 0;
			dy = -1;
			break;
		case Player::Left:
			dx = -1;
			dy = 0;
			break;
		case Player::Down:
			dx = 0;
			dy = 1;
			break;
	}
	int xnew, ynew;
	xnew = ((p->XPos + dx) + (FIELD_WIDTH * TILE_SIZE)) % (FIELD_WIDTH * TILE_SIZE);
	ynew = ((p->YPos + dy) + (FIELD_HEIGHT * TILE_SIZE)) % (FIELD_HEIGHT * TILE_SIZE);

	if (GameField.Tiles[xnew / TILE_SIZE][ynew / TILE_SIZE] & Field::Empty)
	{
		p->XPos = xnew;
		p->YPos = ynew;
		return true;
	}
	else
	{
		return false;
	}
}
