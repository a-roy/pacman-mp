//! \file
//! Player class declaration

#pragma once

#include "Animation.h"

class Player
{
	public:
		enum Direction
		{
			Right,
			Up,
			Left,
			Down
		};
	
		int XPos;
		int YPos;
		Direction CurrentDir;
		Direction NextDir;
		int AnimFrame;
		std::vector<Animation *> Animations;

		virtual void Draw() = 0;
};

class Pacman : public Player
{
	public:
		Pacman();
		void Draw();
};

class Ghost : public Player
{
	public:
		Ghost();
		void Draw();
};

enum Character
{
	Pacman_c,
	Ghost_c
};
