//! \file
//! Entry point

#include <map>
#include <string>
#include <vector>
#include "MainState.h"
#include "Game.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "NetworkManager.h"
#include "Sprite.h"
#include "Animation.h"
#include "MenuItem.h"

static void update(MainState &state);
static void render(const MainState &state);
static void change(MainState &state, MainState nextState);
static void addrIncrement(unsigned int digit, int amount);

Menu menu;
unsigned int index = 0;
Game* game;
Field f;
std::map<std::string, int> TextMap;
std::vector<std::string> Menu_EN;
unsigned char ip[4];
unsigned short port;
Sprite sprite;
int frame;

int text;

int main()
{
	Menu_EN.push_back("Host game");
	Menu_EN.push_back("Join game");
	Menu_EN.push_back("Quit");

	std::string font = "../prstartk.ttf";

	TextMap["."] = Renderer::CreateText(font, ".", 24);
	TextMap[":"] = Renderer::CreateText(font, ":", 24);
	TextMap["0"] = Renderer::CreateText(font, "0", 24);
	TextMap["1"] = Renderer::CreateText(font, "1", 24);
	TextMap["2"] = Renderer::CreateText(font, "2", 24);
	TextMap["3"] = Renderer::CreateText(font, "3", 24);
	TextMap["4"] = Renderer::CreateText(font, "4", 24);
	TextMap["5"] = Renderer::CreateText(font, "5", 24);
	TextMap["6"] = Renderer::CreateText(font, "6", 24);
	TextMap["7"] = Renderer::CreateText(font, "7", 24);
	TextMap["8"] = Renderer::CreateText(font, "8", 24);
	TextMap["9"] = Renderer::CreateText(font, "9", 24);

	TextMap[">"] = Renderer::CreateText(font, ">", 24);
	for (unsigned int i = 0; i < Menu_EN.size(); i++)
	{
		std::string string = Menu_EN[i];
		TextMap[string] = Renderer::CreateText(font, string, 24);
	}
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

	Renderer::Scale = 8.0f;
	Renderer::CreateWindow(1280, 720, "My window");

	sprite.Index = Renderer::CreateSprite("../pacman.png");

	text = Renderer::CreateText(font, "TESTING", 24);

	Animation pac_move(4);
	pac_move.AddFrame(34, 170, 32, 32);
	pac_move.AddFrame(1, 170, 32, 32);
	pac_move.AddFrame(34, 170, 32, 32);
	pac_move.AddFrame(67, 170, 32, 32);
	sprite.Animations.push_back(pac_move);
	frame = 0;

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

	NetworkManager::Init();

    while (Renderer::WindowOpen())
    {
		InputHandler::PollEvents();
		if (InputHandler::WindowClosed)
		{
			break;
		}

		update(state);
		render(state);
		if (state == Exiting)
		{
			break;
		}
    }

	Renderer::Deinit();
    return 0;
}

static void update(MainState &state)
{
	switch (state)
	{
		case MainMenu:
			{
				if (InputHandler::InputTime == 0)
				{
					switch (InputHandler::LastInput)
					{
						case Player::Up:
							{
								if (index > 0)
								{
									index--;
								}
							}
							break;
						case Player::Down:
							{
								if (index < menu.size() - 1)
								{
									index++;
								}
							}
							break;
						case Player::Right:
							{
								MainState newState = menu[index].Function;
								if (newState == Host)
								{
									change(state, Host);
								}
								else if (newState == Join)
								{
									change(state, Join);
								}
								else if (newState == Exiting)
								{
									change(state, Exiting);
								}
							}
							break;
					}
				}
			}
			break;
		case Host:
			{
				// handle incoming network traffic
				NetworkManager::MessageType mtype;
				std::vector<char> data;
				unsigned int id;
				NetworkManager::Receive(mtype, data, id);
				switch (mtype)
				{
					case NetworkManager::None:
						break;
					case NetworkManager::RequestServer:
						{
							// TODO: add identity to lobby
							std::vector<char> d(0);
							NetworkManager::Send(NetworkManager::ConfirmClient, d, id);
							change(state, Gameplay);
						}
						break;
					case NetworkManager::PingServer:
						// TODO: confirm identity, reset timeout
						break;
					case NetworkManager::DisconnectServer:
						// TODO: remove identity from lobby
						break;
				}
				if (InputHandler::InputTime == 0 && InputHandler::LastInput == Player::Right)
				{
					change(state, Gameplay);
				}
				else
				{
					// TODO: ping all clients
				}
			}
			break;
		case Join:
			{
				if (InputHandler::InputTime == 0)
				{
					switch (InputHandler::LastInput)
					{
						case Player::Left:
							{
								if (index > 0)
								{
									index--;
								}
							}
							break;
						case Player::Right:
							{
								if (index < 16)
								{
									index++;
								}
								else
								{
									char addr[16];
									sprintf(addr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
									std::string address = addr;
									NetworkManager::ResetConnections();
									unsigned int server = NetworkManager::GetConnection(addr, port);
									// TODO: set connection to waiting
									std::vector<char> data(0);
									NetworkManager::Send(NetworkManager::RequestServer, data, server);
								}
							}
							break;
						case Player::Up:
							{
								addrIncrement(index, 1);
							}
							break;
						case Player::Down:
							{
								addrIncrement(index, -1);
							}
							break;
					}
				}
				// TODO: check connection status
				bool connection_waiting = false;
				// parse server messages
				NetworkManager::MessageType mtype;
				std::vector<char> data;
				unsigned int sender;
				NetworkManager::Receive(mtype, data, sender);
				if (sender == 0)
				{
					switch (mtype)
					{
						case NetworkManager::None:
							// ignore
							break;
						case NetworkManager::ConfirmClient:
							// TODO: display confirmation
							change(state, Gameplay);
							break;
						case NetworkManager::PingClient:
							// TODO: reset timeout
							break;
						case NetworkManager::DisconnectClient:
							// TODO: exit lobby
							break;
					}
				}
				if (connection_waiting)
				{
					bool timeout = false;
					if (timeout)
					{
						// TODO cancel connection
					}
					else
					{
						// send connection request
						std::vector<char> data(0);
						NetworkManager::Send(NetworkManager::RequestServer, data, 0);
					}
				}
				// TODO: ping server
			}
			break;
		case Gameplay:
			{
				game->Players[0].NextDir = InputHandler::LastInput;
				game->update();
			}
			break;
		case Exiting:
			// Don't do anything
			break;
	}
}

static void render(const MainState &state)
{
	Renderer::Clear();

	switch (state)
	{
		case MainMenu:
			{
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
			{
				std::string address = NetworkManager::GetAddress();
				unsigned short port = NetworkManager::GetPort();
				char disp[22];
				sprintf(disp, "%s:%d", address.c_str(), port);
				for (unsigned int i = 0; disp[i] != '\0'; i++)
				{
					std::string s({ disp[i] });
					Renderer::DrawText(TextMap[s], 10 + 6 * i, 15);
				}
			}
			break;
		case Join:
			{
				char disp[22];
				sprintf(disp, "%03d.%03d.%03d.%03d:%05d", ip[0], ip[1], ip[2], ip[3], port);
				for (unsigned int i = 0; i < 21; i++)
				{
					std::string s({ disp[i] });
					Renderer::DrawText(TextMap[s], 10 + 6 * i, 15);
				}
			}
			break;
		case Gameplay:
			{
				Renderer::DrawSprite(
						sprite,
						game->Players[0].XPos, game->Players[0].YPos,
						game->Players[0].CurrentDir * -90.f,
						0, frame++);
				Renderer::DrawText(text, 10, 2);
			}
			break;
		case Exiting:
			// Don't do anything
			break;
	}
	Renderer::Display();
}

static void change(MainState &state, MainState nextState)
{
	state = nextState;
	switch (nextState)
	{
		case MainMenu:
			{
				index = 0;
			}
			break;
		case Host:
			{
				// TODO
			}
			break;
		case Join:
			{
				index = 0;
				ip[0] = 127;
				ip[1] = 0;
				ip[2] = 0;
				ip[3] = 1;
				port = 1;
			}
			break;
		case Gameplay:
			{
				std::vector<Player> p(1, Player(Player::Pacman));
				game = new Game(f, p);
				// TODO: delete game;
			}
			break;
		case Exiting:
			break;
	}
}

static void addrIncrement(unsigned int digit, int amount)
{
	int delta = amount;
	if (digit < 12)
	{
		for (unsigned int i = digit % 3; i < 2; i++)
		{
			delta *= 10;
		}
		unsigned char &selected = ip[digit / 3];
		if (selected + delta == (int)(unsigned char)(selected + delta))
		{
			selected += delta;
		}
		else
		{
			selected = (amount > 0) ? 255 : 0;
		}
	}
	else
	{
		for (unsigned int i = digit - 12; i < 4; i++)
		{
			delta *= 10;
		}
		if (port + delta == (int)(unsigned short)(port + delta))
		{
			port += delta;
		}
		else
		{
			port = (amount > 0) ? 65535 : 0;
		}
	}
}
