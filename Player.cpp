#include "Player.h"
#include "Field.h"

Player::Player(Character c)
{
	if (c == Pacman)
	{
		XPos = 10 * TILE_SIZE;
		YPos = 12 * TILE_SIZE;
	}
	else if (c == Ghost)
	{
		XPos = 10 * TILE_SIZE;
		YPos = 10 * TILE_SIZE;
	}
	Chara = c;
	CurrentDir = Right;
	NextDir = Right;
}
