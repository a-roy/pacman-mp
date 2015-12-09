#include "Player.h"
#include "Renderer.h"

Ghost::Ghost(const Sprite &playerSprite,
		Position startingPos, Position startingDir) :
	Player(playerSprite, startingPos, startingDir)
{
	Fear = 0;
}

bool Ghost::CanGo(const Field *f, Position delta)
{
	if (delta == CurrentDir * -1)
	{
		return false;
	}
	else
	{
		return Player::CanGo(f, delta);
	}
}

Player::Event Ghost::Move(const Field *f, Field::PelletStatus &p)
{
	if (Fear > 0)
	{
		Fear--;
	}
	if (Dying > 0)
	{
		Dying--;
		return None;
	}
	else if (Dying == 0)
	{
		Reset();
		return None;
	}

	return Player::Move(f, p);
}

int Ghost::Speed()
{
	int x_tile = CurrentPos.X / TILE_SIZE;
	int y_tile = CurrentPos.Y / TILE_SIZE;
	if (y_tile == 14 && (x_tile < 5 || x_tile >= FIELD_WIDTH - 5))
	{
		return 5;
	}
	else if (Fear > 0)
	{
		return 6;
	}
	else
	{
		return 10;
	}
}

Player::Event Ghost::CollideWith(const Player *other)
{
	const Pacman *pacman = dynamic_cast<const Pacman *>(other);

	if (pacman != NULL)
	{
		if (Fear > 0)
		{
			Dying = 0;
			return GhostDied;
		}
		else if (pacman->Dying < 0)
		{
			Reset();
		}
	}

	return None;
}

void Ghost::ProcessEvent(Player::Event event)
{
	if (event & PacmanPowered)
	{
		Fear = 300;
	}
	if (event & PacmanDied)
	{
		Reset();
	}
}

void Ghost::Reset()
{
	CurrentPos = StartingPos;
	CurrentDir = Up;
	NextDir = Up;
	AnimFrame = 0;
	Fear = 0;
	Paused = 68;
	Dying = -1;
}

void Ghost::Draw() const
{
	int anim = 0;
	bool flip = false;
	if (Fear > 0)
	{
		anim = 3;
	}
	else
	{
		if (CurrentDir == Right)
		{
			flip = true;
		}
		else if (CurrentDir == Up)
		{
			anim = 1;
		}
		else if (CurrentDir == Down)
		{
			anim = 2;
		}
	}
	Renderer::DrawSprite(
			PlayerSprite,
			CurrentPos.X, CurrentPos.Y, 0.f, flip, anim, AnimFrame);
}
