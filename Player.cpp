#include "Player.h"
#include "Field.h"
#include "Renderer.h"
#include "Data.h"

Pacman::Pacman()
{
	XPos = 10 * TILE_SIZE;
	YPos = 12 * TILE_SIZE;
	CurrentDir = Right;
	NextDir = Right;
}

void Pacman::Draw()
{
	Renderer::DrawSprite(
			Data::GameplayData.PacmanSprite,
			XPos, YPos,
			CurrentDir * -90.f,
			true, 0, AnimFrame);
}

Ghost::Ghost()
{
	XPos = 10 * TILE_SIZE;
	YPos = 10 * TILE_SIZE;
	CurrentDir = Right;
	NextDir = Right;
}

void Ghost::Draw()
{
	int anim = 0;
	bool flip = false;
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
	Renderer::DrawSprite(
			Data::GameplayData.GhostSprite,
			XPos, YPos, 0.f, flip, anim, AnimFrame);
}
