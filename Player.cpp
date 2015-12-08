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
		for (int i = 0, distance = Speed(); i < distance; i++)
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
