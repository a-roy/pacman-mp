#include "Player.h"
#include "Renderer.h"

void Player::SetDirection(Position direction)
{
	if (!Cornering)
	{
		NextDir = direction;
	}
}

bool Player::CanGo(const Field *f, Position delta)
{
	Position sum = CurrentPos + delta;
	Position newPos(
			(sum.X + (FIELD_WIDTH * TILE_SIZE))
			% (FIELD_WIDTH * TILE_SIZE),
			(sum.Y + (FIELD_HEIGHT * TILE_SIZE))
			% (FIELD_HEIGHT * TILE_SIZE));
	Field::TileType new_tile = f->InterpolateAtPos(newPos);

	Field::TileType move_flag = MoveFlag();
	return (new_tile & move_flag) == move_flag;
}

Player::Event Player::Move(const Field *f, Field::PelletStatus &p)
{
	if (Paused == 0)
	{
		int corner_range = CornerRange();
		for (int i = 0; i < Speed; i++)
		{
			if (CanGo(f, NextDir))
			{
				CurrentPos += NextDir;
				CurrentDir = NextDir;
				Cornering = false;
			}
			else if (Cornering
					|| corner_range > 0
					&& CanGo(
						f,
						CurrentDir * CornerRange() + NextDir * CornerRange()))
			{
				CurrentPos += CurrentDir + NextDir;
				Cornering = true;
			}
			else if (CanGo(f, CurrentDir))
			{
				CurrentPos += CurrentDir;
			}
			CurrentPos = Position(
					(CurrentPos.X + (FIELD_WIDTH * TILE_SIZE))
					% (FIELD_WIDTH * TILE_SIZE),
					(CurrentPos.Y + (FIELD_HEIGHT * TILE_SIZE))
					% (FIELD_HEIGHT * TILE_SIZE));
		}
	}
	else
	{
		Paused--;
	}
	AnimFrame++;
	return None;
}

Sprite Pacman::PacmanSprite;

Pacman::Pacman()
{
	CurrentPos = Position(
			13 * TILE_SIZE + (TILE_SIZE - 1) / 2,
			23 * TILE_SIZE + (TILE_SIZE - 1) / 2);
	Speed = 11;
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

Sprite Ghost::GhostSprite;

Ghost::Ghost()
{
	CurrentPos = Position(
			13 * TILE_SIZE + (TILE_SIZE - 1) / 2,
			11 * TILE_SIZE + (TILE_SIZE - 1) / 2);
	Speed = 10;
	Paused = 180;
	Cornering = false;
	CurrentDir = Left;
	NextDir = Left;
	AnimFrame = 0;
	Fear = 0;
}

bool Ghost::CanGo(const Field *f, Position delta)
{
	if (delta == CurrentDir * -1)
	{
		return false;
	}
	else
	{
		return Player::CanGo(f, delta);
	}
}

Player::Event Ghost::Move(const Field *f, Field::PelletStatus &p)
{
	if (Fear > 0)
	{
		Fear--;
	}

	return Player::Move(f, p);
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
	CurrentPos = Position(
			13 * TILE_SIZE + (TILE_SIZE - 1) / 2,
			13 * TILE_SIZE + (TILE_SIZE - 1) / 2);
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
			CurrentPos.X, CurrentPos.Y, 0.f, flip, anim, AnimFrame);
}
