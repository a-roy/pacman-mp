#include "Player.h"
#include "Renderer.h"

Player::Player(const Sprite &playerSprite,
		Position startingPos, Position startingDir)
{
	PlayerSprite = playerSprite;
	StartingPos = startingPos;
	CurrentPos = StartingPos;
	CurrentDir = startingDir;
	NextDir = CurrentDir;
	Paused = 180;
	Dying = -1;
	Cornering = false;
	AnimFrame = 0;
}

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
		bool pos_changed = false;
		for (int i = 0, distance = Speed(); i < distance; i++)
		{
			if (CanGo(f, NextDir))
			{
				CurrentPos += NextDir;
				CurrentDir = NextDir;
				Cornering = false;
				pos_changed = true;
			}
			else if (Cornering
					|| corner_range > 0
					&& CanGo(
						f,
						CurrentDir * CornerRange() + NextDir * CornerRange()))
			{
				CurrentPos += CurrentDir + NextDir;
				Cornering = true;
				pos_changed = true;
			}
			else if (CanGo(f, CurrentDir))
			{
				CurrentPos += CurrentDir;
				pos_changed = true;
			}
			CurrentPos = Position(
					(CurrentPos.X + (FIELD_WIDTH * TILE_SIZE))
					% (FIELD_WIDTH * TILE_SIZE),
					(CurrentPos.Y + (FIELD_HEIGHT * TILE_SIZE))
					% (FIELD_HEIGHT * TILE_SIZE));
		}
		if (pos_changed)
		{
			AnimFrame++;
		}
	}
	else
	{
		Paused--;
	}
	return None;
}
