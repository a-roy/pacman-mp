#include "Player.h"
#include "Renderer.h"

void Player::Move(const Field *f, Field::PelletStatus &p)
{
	for (int i = 0; i < Speed; i++)
	{
		if (Move(f, NextDir))
		{
			CurrentDir = NextDir;
		}
		else
		{
			Move(f, CurrentDir);
		}
	}
}

bool Player::Move(const Field *f, Direction d)
{
	int dx, dy;
	switch (d)
	{
		case Right:
			dx = 1;
			dy = 0;
			break;
		case Up:
			dx = 0;
			dy = -1;
			break;
		case Left:
			dx = -1;
			dy = 0;
			break;
		case Down:
			dx = 0;
			dy = 1;
			break;
	}
	int xnew, ynew;
	xnew = ((XPos + dx) + (FIELD_WIDTH * TILE_SIZE))
		% (FIELD_WIDTH * TILE_SIZE);
	ynew = ((YPos + dy) + (FIELD_HEIGHT * TILE_SIZE))
		% (FIELD_HEIGHT * TILE_SIZE);
	Field::TileType new_tile = f->InterpolateAtPos(xnew, ynew);

	if (new_tile & Field::Empty)
	{
		XPos = xnew;
		YPos = ynew;
		return true;
		CurrentDir = NextDir;
	}
	else
	{
		return false;
	}
}

Sprite Pacman::PacmanSprite;

Pacman::Pacman()
{
	XPos = 13 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	YPos = 23 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	Speed = 11;
	CurrentDir = Left;
	NextDir = Left;
	AnimFrame = 0;
	PoweredUp = 0;
}

void Pacman::Move(const Field *f, Field::PelletStatus &p)
{
	Player::Move(f, p);
	if (PoweredUp > 0)
	{
		PoweredUp--;
	}
	Field::TileType tile = f->Tiles[XPos / TILE_SIZE][YPos / TILE_SIZE];
	if ((tile & Field::Pellet) == Field::Pellet)
	{
		if (!p.IsEaten(XPos / TILE_SIZE, YPos / TILE_SIZE))
		{
			p.Eat(XPos / TILE_SIZE, YPos / TILE_SIZE);
			if (tile == Field::PowerPellet)
			{
				PoweredUp = 300;
			}
		}
	}
}

void Pacman::CollideWith(const Player *other)
{
	const Ghost *ghost = dynamic_cast<const Ghost *>(other);

	if (ghost != NULL)
	{
		if (PoweredUp == 0)
		{
			Reset();
		}
	}
}

void Pacman::Reset()
{
	XPos = 13 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	YPos = 23 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	CurrentDir = Left;
	NextDir = Left;
	PoweredUp = 0;
}

void Pacman::Draw(int fear) const
{
	Renderer::DrawSprite(
			PacmanSprite,
			XPos, YPos,
			CurrentDir * -90.f,
			true, 0, AnimFrame);
}

Sprite Ghost::GhostSprite;

Ghost::Ghost()
{
	XPos = 13 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	YPos = 11 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	Speed = 10;
	CurrentDir = Left;
	NextDir = Left;
	AnimFrame = 0;
}

bool Ghost::Move(const Field *f, Direction d)
{
	if ((CurrentDir == Right && d == Left)
			|| (CurrentDir == Up && d == Down)
			|| (CurrentDir == Left && d == Right)
			|| (CurrentDir == Down && d == Up))
	{
		return false;
	}
	else
	{
		return Player::Move(f, d);
	}
}

void Ghost::CollideWith(const Player *other)
{
	const Pacman *pacman = dynamic_cast<const Pacman *>(other);

	if (pacman != NULL)
	{
		Reset();
	}
}

void Ghost::Reset()
{
	XPos = 13 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	YPos = 13 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	CurrentDir = Up;
	NextDir = Up;
	AnimFrame = 0;
}

void Ghost::Draw(int fear) const
{
	int anim = 0;
	bool flip = false;
	if (fear > 0)
	{
		anim = 3;
	}
	else
	{
		if (CurrentDir == Right)
		{
			flip = true;
		}
		else if (CurrentDir == Up)
		{
			anim = 1;
		}
		else if (CurrentDir == Down)
		{
			anim = 2;
		}
	}
	Renderer::DrawSprite(
			GhostSprite,
			XPos, YPos, 0.f, flip, anim, AnimFrame);
}
