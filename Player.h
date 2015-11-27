//! \file
//! Player class declaration

#pragma once

#include "Field.h"
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

		virtual void Move(Field *f);
		virtual bool Move(Field *f, Direction d);
		virtual void Draw() = 0;
		virtual Player *Clone() = 0;
};

class Pacman : public Player
{
	public:
		Pacman();
		void Draw();
		Player *Clone() { return new Pacman(*this); }
};

class Ghost : public Player
{
	public:
		Ghost();
		bool Move(Field *f, Direction d);
		void Draw();
		Player *Clone() { return new Ghost(*this); }
};

enum Character
{
	Pacman_c,
	Ghost_c
};
