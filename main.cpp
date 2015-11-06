//! \file
//! Entry point

#include <map>
#include <string>
#include <vector>
#include "MainState.h"
#include "Game.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Sprite.h"
#include "Animation.h"
#include "MenuItem.h"

int main()
{
	std::vector<std::string> Menu_EN;
	Menu_EN.push_back("Host game");
	Menu_EN.push_back("Join game");
	Menu_EN.push_back("Quit");

	std::string font = "../prstartk.ttf";

	std::map<std::string, int> TextMap;
	TextMap[">"] = Renderer::CreateText(font, ">", 24);
	for (unsigned int i = 0; i < Menu_EN.size(); i++)
	{
		std::string string = Menu_EN[i];
		TextMap[string] = Renderer::CreateText(font, string, 24);
	}
	Menu menu;
	MenuItem item1, item2, item3;
	item1.Text = 0;
	item1.Function = Host;
	menu.push_back(item1);
	item2.Text = 1;
	item2.Function = Join;
	menu.push_back(item2);
	item3.Text = 2;
	item3.Function = Exiting;
	menu.push_back(item3);

	unsigned int index = 0;

	Renderer::Scale = 8.0f;
	Renderer::CreateWindow(1280, 720, "My window");

	Sprite sprite;
	sprite.Index = Renderer::CreateSprite("../pacman.png");

	int text = Renderer::CreateText(font, "TESTING", 24);

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

	MainState state = MainMenu;

	//! \todo Display the menu here

	std::vector<Player> p(1, Player(Player::Pacman));
	Game g(f, p);

    while (Renderer::WindowOpen())
    {
		InputHandler::PollEvents();
		if (InputHandler::WindowClosed)
		{
			Renderer::Deinit();
			return 0;
		}

		switch (state)
		{
			case MainMenu:
				if (InputHandler::InputTime == 0)
				{
					switch (InputHandler::LastInput)
					{
						case Player::Up:
							if (index > 0)
							{
								index--;
							}
							break;
						case Player::Down:
							if (index < menu.size() - 1)
							{
								index++;
							}
							break;
						case Player::Right:
							MainState newState = menu[index].Function;
							if (newState == Host)
							{
								state = Gameplay;
							}
							else if (newState == Exiting)
							{
								state = Exiting;
							}
							break;
					}
				}
				else
				{
					Renderer::Clear();
					for (unsigned int i = 0; i < menu.size(); i++)
					{
						Renderer::DrawText(
								TextMap[Menu_EN[menu[i].Text]], 6, 10 + 6 * i);
						if (i == index)
						{
							Renderer::DrawText(
									TextMap[">"], 2, 10 + 6 * i);
						}
					}
				}
				break;
			case Host:
				// TODO: display IP address and port
				// TODO: wait for connection requests
				// TODO: start gameplay
				break;
			case Join:
				// TODO: prompt for IP address and port
				// TODO: send connection request
				// TODO: parse server messages
				break;
			case Gameplay:
				g.Players[0].NextDir = InputHandler::LastInput;
				g.update();
				Renderer::Clear();

				// draw
				Renderer::DrawSprite(
						sprite,
						g.Players[0].XPos, g.Players[0].YPos,
						g.Players[0].CurrentDir * -90.f,
						0, frame++);
				Renderer::DrawText(text, 10, 2);
				break;
			case Exiting:
				Renderer::Deinit();
				return 0;
		}
		Renderer::Display();
    }

    return 0;
}
