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
			0, AnimFrame);
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
	if (CurrentDir == Right || CurrentDir == Left)
	{
		anim = 0;
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
			XPos, YPos, 0.f, anim, AnimFrame);
}
