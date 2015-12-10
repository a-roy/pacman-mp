#include "Player.h"
#include "Renderer.h"

Pacman::Pacman(const Sprite &playerSprite,
		Position startingPos, Position startingDir) :
	Player(playerSprite, startingPos, startingDir)
{ }

Player::Event Pacman::Update(const Field *f, Field::PelletStatus &p)
{
	if (Dying > 0)
	{
		if (Dying == 52)
		{
			AnimFrame = 0;
		}
		else if (Dying < 52)
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

	Player::Update(f, p);
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

int Pacman::Speed(const Field *f) const
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
			Dying = 68;
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
	CurrentPos = StartingPos;
	CurrentDir = Left;
	NextDir = Left;
	Dying = -1;
}

void Pacman::Draw() const
{
	bool die_anim = (Dying >= 0 && Dying <= 52);
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
			PlayerSprite,
			CurrentPos.X, CurrentPos.Y,
			rotation,
			true, anim, AnimFrame);
}
