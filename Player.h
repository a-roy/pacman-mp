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
		enum Event
		{
			None,
			PacmanPowered,
			PacmanDied
		};

		int XPos;
		int YPos;
		int Speed;
		int Starting;
		Direction CurrentDir;
		Direction NextDir;
		int AnimFrame;
		std::vector<Animation *> Animations;

		virtual Event Move(const Field *f, Field::PelletStatus &p);
		virtual bool Move(const Field *f, Direction d);
		virtual Event CollideWith(const Player *other) = 0;
		virtual void ProcessEvent(Event event) = 0;
		virtual void Reset() = 0;
		virtual Field::TileType MoveFlag() = 0;
		virtual void Draw() const = 0;
		virtual Player *Clone() = 0;
};

class Pacman : public Player
{
	public:
		static Sprite PacmanSprite;
		int Dying;

		Pacman();
		Event Move(const Field *f, Field::PelletStatus &p);
		Event CollideWith(const Player *other);
		void ProcessEvent(Event event);
		void Reset();
		void Draw() const;
		Field::TileType MoveFlag() { return Field::PacmanZone; }
		Player *Clone() { return new Pacman(*this); }
};

class Ghost : public Player
{
	public:
		static Sprite GhostSprite;
		int Fear;

		Ghost();
		Event Move(const Field *f, Field::PelletStatus &p);
		bool Move(const Field *f, Direction d);
		Event CollideWith(const Player *other);
		void ProcessEvent(Event event);
		void Reset();
		void Draw() const;
		Field::TileType MoveFlag() { return Field::GhostZone; }
		Player *Clone() { return new Ghost(*this); }
};

enum Character
{
	Pacman_c,
	Ghost_c
};
