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
			None = 0x00,
			PacmanPowered = 0x01,
			PacmanDied = 0x02,
			PacmanRespawned = 0x04,
			GhostDied = 0x08
		};

		Position CurrentPos;
		int Paused;
		int Dying;
		bool Cornering;
		Position StartingPos;
		Position CurrentDir;
		Position NextDir;
		int AnimFrame;
		std::vector<Animation *> Animations;
		Sprite PlayerSprite;

		Player(const Sprite &playerSprite,
				Position startingPos, Position startingDir);
		void SetDirection(Position direction);
		virtual bool CanGo(const Field *f, Position delta);
		virtual Event Move(const Field *f, Field::PelletStatus &p);
		virtual int Speed() = 0;
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
		Pacman(const Sprite &playerSprite,
				Position startingPos, Position startingDir);
		Event Move(const Field *f, Field::PelletStatus &p);
		int Speed();
		int CornerRange() { return TILE_SIZE / 2; }
		Field::TileType MoveFlag() { return Field::PacmanZone; }
		Event CollideWith(const Player *other);
		void ProcessEvent(Event event);
		void Reset();
		void Draw() const;
		Player *Clone() { return new Pacman(*this); }
};

class Ghost : public Player
{
	public:
		int Fear;

		Ghost(const Sprite &playerSprite,
				Position startingPos, Position startingDir);
		bool CanGo(const Field *f, Position delta);
		Event Move(const Field *f, Field::PelletStatus &p);
		int Speed();
		int CornerRange() { return 0; }
		Field::TileType MoveFlag() { return Field::GhostZone; }
		Event CollideWith(const Player *other);
		void ProcessEvent(Event event);
		void Reset();
		void Draw() const;
		Player *Clone() { return new Ghost(*this); }
};

enum Character
{
	PacMan,
	Blinky,
	Inky,
	Pinky,
	Clyde
};
