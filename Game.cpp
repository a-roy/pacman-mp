#include "Game.h"

Game::Game(Field f, std::vector<Player *> p)
{
	GameField = f;
	Players = p;
	CurrentFrame = 0;
}

Game::~Game()
{
	for (unsigned int i = 0, size = Players.size(); i < size; i++)
	{
		delete Players[i];
	}
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
