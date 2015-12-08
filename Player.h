//! \file
//! Player class declaration

#pragma once

#include <array>
#include <vector>
#include "Position.h"
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
			PacmanDied,
			PacmanRespawned
		};

		Position CurrentPos;
		int Speed;
		int Paused;
		bool Cornering;
		Position CurrentDir;
		Position NextDir;
		int AnimFrame;
		std::vector<Animation *> Animations;

		void SetDirection(Position direction);
		virtual bool CanGo(const Field *f, Position delta);
		virtual Event Move(const Field *f, Field::PelletStatus &p);
		virtual int CornerRange() = 0;
		virtual Field::TileType MoveFlag() = 0;
		virtual Event CollideWith(const Player *other) = 0;
		virtual void ProcessEvent(Event event) = 0;
		virtual void Reset() = 0;
		virtual void Draw() const = 0;
		virtual Player *Clone() = 0;
};

class Pacman : public Player
{
	public:
		static Sprite PacmanSprite;
		int Dying;

		Pacman();
		int CornerRange() { return TILE_SIZE / 2; }
		Field::TileType MoveFlag() { return Field::PacmanZone; }
		Event Move(const Field *f, Field::PelletStatus &p);
		Event CollideWith(const Player *other);
		void ProcessEvent(Event event);
		void Reset();
		void Draw() const;
		Player *Clone() { return new Pacman(*this); }
};

class Ghost : public Player
{
	public:
		static Sprite GhostSprite;
		int Fear;

		Ghost();
		int CornerRange() { return 0; }
		bool CanGo(const Field *f, Position delta);
		Field::TileType MoveFlag() { return Field::GhostZone; }
		Event Move(const Field *f, Field::PelletStatus &p);
		Event CollideWith(const Player *other);
		void ProcessEvent(Event event);
		void Reset();
		void Draw() const;
		Player *Clone() { return new Ghost(*this); }
};

enum Character
{
	Pacman_c,
	Ghost_c
};
