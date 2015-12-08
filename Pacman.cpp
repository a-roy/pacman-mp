#include "Player.h"
#include "Renderer.h"

Sprite Pacman::PacmanSprite;

Pacman::Pacman()
{
	CurrentPos = Position(
			14 * TILE_SIZE,
			23 * TILE_SIZE + (TILE_SIZE - 1) / 2);
	Paused = 180;
	Cornering = false;
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
	Field::TileType tile =
		f->Tiles[CurrentPos.X / TILE_SIZE][CurrentPos.Y / TILE_SIZE];
	if ((tile & Field::Pellet) == Field::Pellet)
	{
		if (!p.IsEaten(CurrentPos.X / TILE_SIZE, CurrentPos.Y / TILE_SIZE))
		{
			p.Eat(CurrentPos.X / TILE_SIZE, CurrentPos.Y / TILE_SIZE);
			if (tile == Field::Pellet)
			{
				Paused += 1;
				return None;
			}
			if (tile == Field::PowerPellet)
			{
				Paused += 3;
				return PacmanPowered;
			}
		}
	}
	return None;
}

int Pacman::Speed()
{
	return 11;
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
	CurrentPos = Position(
			13 * TILE_SIZE + (TILE_SIZE - 1) / 2,
			23 * TILE_SIZE + (TILE_SIZE - 1) / 2);
	CurrentDir = Left;
	NextDir = Left;
	Dying = -1;
}

void Pacman::Draw() const
{
	bool die_anim = (Dying > 0 && Dying <= 48);
	float rotation = 0.f;
	int anim;
	if (die_anim)
	{
		rotation = 0.f;
		anim = 1;
	}
	else
	{
		if (CurrentDir == Right)
		{
			rotation = 0.f;
		}
		else if (CurrentDir == Down)
		{
			rotation = 90.f;
		}
		else if (CurrentDir == Left)
		{
			rotation = 180.f;
		}
		else if (CurrentDir == Up)
		{
			rotation = -90.f;
		}
		anim = 0;
	}
	Renderer::DrawSprite(
			PacmanSprite,
			CurrentPos.X, CurrentPos.Y,
			rotation,
			true, anim, AnimFrame);
}
