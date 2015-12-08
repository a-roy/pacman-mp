#include "Player.h"
#include "Renderer.h"

Player::Event Player::Move(const Field *f, Field::PelletStatus &p)
{
	if (Paused == 0)
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
	else
	{
		Paused--;
	}
	AnimFrame++;
	return None;
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

	Field::TileType move_flag = MoveFlag();
	if ((new_tile & move_flag) == move_flag)
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
	Paused = 180;
	CurrentDir = Left;
	NextDir = Left;
	AnimFrame = 0;
	Dying = -1;
}

Player::Event Pacman::Move(const Field *f, Field::PelletStatus &p)
{
	if (Dying > 0)
	{
		if (Dying == 48)
		{
			AnimFrame = 0;
		}
		else if (Dying < 48)
		{
			AnimFrame++;
		}
		Dying--;
		return None;
	}
	else if (Dying == 0)
	{
		Reset();
		return PacmanRespawned;
	}

	Player::Move(f, p);
	Field::TileType tile = f->Tiles[XPos / TILE_SIZE][YPos / TILE_SIZE];
	if ((tile & Field::Pellet) == Field::Pellet)
	{
		if (!p.IsEaten(XPos / TILE_SIZE, YPos / TILE_SIZE))
		{
			p.Eat(XPos / TILE_SIZE, YPos / TILE_SIZE);
			if (tile == Field::Pellet)
			{
				Paused = 1;
				return None;
			}
			if (tile == Field::PowerPellet)
			{
				Paused = 3;
				return PacmanPowered;
			}
		}
	}
	return None;
}

Player::Event Pacman::CollideWith(const Player *other)
{
	const Ghost *ghost = dynamic_cast<const Ghost *>(other);

	if (ghost != NULL && Dying < 0)
	{
		if (ghost->Fear == 0)
		{
			Dying = 64;
			return PacmanDied;
		}
	}

	return None;
}

void Pacman::ProcessEvent(Player::Event event)
{
}

void Pacman::Reset()
{
	XPos = 13 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	YPos = 23 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	CurrentDir = Left;
	NextDir = Left;
	Dying = -1;
}

void Pacman::Draw() const
{
	bool die_anim = (Dying > 0 && Dying <= 48);
	Renderer::DrawSprite(
			PacmanSprite,
			XPos, YPos,
			die_anim ? 0 : (CurrentDir * -90.f),
			true, die_anim ? 1 : 0, AnimFrame);
}

Sprite Ghost::GhostSprite;

Ghost::Ghost()
{
	XPos = 13 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	YPos = 11 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	Speed = 10;
	Paused = 180;
	CurrentDir = Left;
	NextDir = Left;
	AnimFrame = 0;
	Fear = 0;
}

Player::Event Ghost::Move(const Field *f, Field::PelletStatus &p)
{
	if (Fear > 0)
	{
		Fear--;
	}

	return Player::Move(f, p);
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

Player::Event Ghost::CollideWith(const Player *other)
{
	const Pacman *pacman = dynamic_cast<const Pacman *>(other);

	if (pacman != NULL)
	{
		if (Fear > 0)
		{
			Reset();
		}
		else if (pacman->Dying < 0)
		{
			Reset();
		}
	}

	return None;
}

void Ghost::ProcessEvent(Player::Event event)
{
	if (event == PacmanPowered)
	{
		Fear = 300;
	}
	else if (event == PacmanDied)
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
	Fear = 0;
	Paused = 64;
}

void Ghost::Draw() const
{
	int anim = 0;
	bool flip = false;
	if (Fear > 0)
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
