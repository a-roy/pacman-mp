//! \file
//! Player class declaration

#pragma once

class Player
{
public:
	//
	enum Character
	{
		Pacman,
		Ghost
	};
	enum Direction
	{
		Right,
		Up,
		Left,
		Down
	};

	Character Chara;
	int XPos;
	int YPos;
	Direction CurrentDir;
	Direction NextDir;

	Player(Character c);
};
