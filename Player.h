//! \file
//! Player class declaration

#pragma once

#include <array>
#include <vector>
#include "Direction.h"
#include "Field.h"
#include "Sprite.h"
#include "Animation.h"

class Player
{
	public:
		int XPos;
		int YPos;
		int Speed;
		Direction CurrentDir;
		Direction NextDir;
		int AnimFrame;
		std::vector<Animation *> Animations;

		virtual void Move(const Field *f, Field::PelletStatus &p);
		virtual bool Move(const Field *f, Direction d);
		virtual void CollideWith(const Player *other) = 0;
		virtual void Reset() = 0;
		virtual void Draw(int fear) const = 0;
		virtual Player *Clone() = 0;
};

class Pacman : public Player
{
	public:
		static Sprite PacmanSprite;
		int PoweredUp;

		Pacman();
		void Move(const Field *f, Field::PelletStatus &p);
		void CollideWith(const Player *other);
		void Reset();
		void Draw(int fear) const;
		Player *Clone() { return new Pacman(*this); }
};

class Ghost : public Player
{
	public:
		static Sprite GhostSprite;

		Ghost();
		bool Move(const Field *f, Direction d);
		void CollideWith(const Player *other);
		void Reset();
		void Draw(int fear) const;
		Player *Clone() { return new Ghost(*this); }
};

enum Character
{
	Pacman_c,
	Ghost_c
};
