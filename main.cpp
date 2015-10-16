//! \file
//! Entry point

#include "Game.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Sprite.h"
#include "Animation.h"

int main()
{
	Renderer::Scale = 8.0f;
	Renderer::CreateWindow(1280, 720, "My window");

	Sprite sprite;
	sprite.Index = Renderer::CreateSprite("../pacman.png");

	Animation pac_move(4);
	pac_move.AddFrame(34, 170, 32, 32);
	pac_move.AddFrame(1, 170, 32, 32);
	pac_move.AddFrame(34, 170, 32, 32);
	pac_move.AddFrame(67, 170, 32, 32);
	sprite.Animations.push_back(pac_move);
	int frame = 0;

	Field f;
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			if (i == 0 || i == FIELD_WIDTH - 1 || j == 0 || j == FIELD_HEIGHT - 1)
			{
				f.Tiles[i][j] = Field::Wall;
			}
			else
			{
				f.Tiles[i][j] = Field::Empty;
			}
		}
	}
	std::vector<Player> p(1, Player(Player::Pacman));
	Game g(f, p);

    while (Renderer::WindowOpen())
    {
		InputHandler::PollEvents();
		g.Players[0].NextDir = InputHandler::LastInput;
		if (InputHandler::WindowClosed)
		{
			Renderer::Deinit();
			return 0;
		}

		g.update();
		Renderer::Clear();

		// draw
		Renderer::DrawSprite(sprite, g.Players[0].XPos, g.Players[0].YPos, g.Players[0].CurrentDir * -90, 0, frame++);

		Renderer::Display();
    }

    return 0;
}
