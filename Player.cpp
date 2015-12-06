#include "Player.h"
#include "Renderer.h"

void Player::Move(const Field *f, PelletStatus &p)
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
	YPos = 17 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	CurrentDir = Right;
	NextDir = Right;
}

void Pacman::Move(const Field *f, PelletStatus &p)
{
	Player::Move(f, p);
	Field::TileType tile = f->Tiles[XPos / TILE_SIZE][YPos / TILE_SIZE];
	if ((tile & Field::Pellet) == Field::Pellet)
	{
		p[YPos / TILE_SIZE] |= (1U << (XPos / TILE_SIZE));
	}
}

void Pacman::Draw()
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
	XPos = 11 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	YPos = 17 * TILE_SIZE + (TILE_SIZE - 1) / 2;
	CurrentDir = Right;
	NextDir = Right;
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

void Ghost::Draw()
{
	int anim = 0;
	bool flip = false;
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
	Renderer::DrawSprite(
			GhostSprite,
			XPos, YPos, 0.f, flip, anim, AnimFrame);
}
