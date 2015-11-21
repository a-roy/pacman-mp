//! \file
//! Entry point

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "MainState.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "NetworkManager.h"
#include "Data.h"
#include "local_update.h"
#include "process_packets.h"

static void update(MainState &state);
static void render(const MainState &state);
static void change(MainState &state, MainState nextState);

int main()
{
	Data::MainMenuData.Menu_EN.push_back("Host game");
	Data::MainMenuData.Menu_EN.push_back("Join game");
	Data::MainMenuData.Menu_EN.push_back("Quit");

	Data::Font = "../prstartk.ttf";

	Data::TextMap["."] = Renderer::CreateText(Data::Font, ".", 24);
	Data::TextMap[":"] = Renderer::CreateText(Data::Font, ":", 24);
	Data::TextMap["0"] = Renderer::CreateText(Data::Font, "0", 24);
	Data::TextMap["1"] = Renderer::CreateText(Data::Font, "1", 24);
	Data::TextMap["2"] = Renderer::CreateText(Data::Font, "2", 24);
	Data::TextMap["3"] = Renderer::CreateText(Data::Font, "3", 24);
	Data::TextMap["4"] = Renderer::CreateText(Data::Font, "4", 24);
	Data::TextMap["5"] = Renderer::CreateText(Data::Font, "5", 24);
	Data::TextMap["6"] = Renderer::CreateText(Data::Font, "6", 24);
	Data::TextMap["7"] = Renderer::CreateText(Data::Font, "7", 24);
	Data::TextMap["8"] = Renderer::CreateText(Data::Font, "8", 24);
	Data::TextMap["9"] = Renderer::CreateText(Data::Font, "9", 24);
	Data::TextMap["^"] = Renderer::CreateText(Data::Font, "^", 24);

	Data::TextMap[">"] = Renderer::CreateText(Data::Font, ">", 24);
	for (unsigned int i = 0; i < Data::MainMenuData.Menu_EN.size(); i++)
	{
		std::string string = Data::MainMenuData.Menu_EN[i];
		Data::TextMap[string] = Renderer::CreateText(Data::Font, string, 24);
	}
	MenuItem item1, item2, item3;
	item1.Text = 0;
	item1.Function = Host;
	Data::MainMenuData.MenuItems.push_back(item1);
	item2.Text = 1;
	item2.Function = Join;
	Data::MainMenuData.MenuItems.push_back(item2);
	item3.Text = 2;
	item3.Function = Exiting;
	Data::MainMenuData.MenuItems.push_back(item3);

	Renderer::Scale = 8.0f;
	Renderer::CreateWindow(1280, 720, "My window");

	Data::GameplayData.PacSprite.Index = Renderer::CreateSprite("../pacman.png");

	Data::ClientsText = Renderer::CreateText(Data::Font, "Clients connected:", 24);

	Data::JoinData.IP[0] = 127;
	Data::JoinData.IP[1] = 0;
	Data::JoinData.IP[2] = 0;
	Data::JoinData.IP[3] = 1;
	Data::JoinData.Port = 0;

	Animation pac_move(4);
	pac_move.AddFrame(34, 170, 32, 32);
	pac_move.AddFrame(1, 170, 32, 32);
	pac_move.AddFrame(34, 170, 32, 32);
	pac_move.AddFrame(67, 170, 32, 32);
	Data::GameplayData.PacSprite.Animations.push_back(pac_move);
	Data::GameplayData.AnimFrame = 0;

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
	MainState s = process_data(state);
	if (s != state)
	{
		change(state, s);
		return;
	}

	s = local_update(state);
	if (s != state)
	{
		change(state, s);
		return;
	}
}

static void render(const MainState &state)
{
	Renderer::Clear();
	std::map<std::string, int> &TextMap = Data::TextMap;

	if (state == MainMenu)
	{
		std::vector<std::string> &Menu_EN = Data::MainMenuData.Menu_EN;
		Menu &menu = Data::MainMenuData.MenuItems;
		unsigned int index = Data::MainMenuData.Index;

		for (unsigned int i = 0; i < menu.size(); i++)
		{
			Renderer::DrawText(
					TextMap[Menu_EN[menu[i].Text]], 6, 10 + 6 * i);
			if (i == index)
			{
				Renderer::DrawText(TextMap[">"], 2, 10 + 6 * i);
			}
		}
	}
	else if (state == Host)
	{
		int lobby_count = Data::HostData.PlayerCount;
		int clients_text = Data::ClientsText;

		std::string address = NetworkManager::GetAddress();
		unsigned short port = NetworkManager::GetPort();
		std::ostringstream ss;
		ss << address << ":" << port;
		std::string str = ss.str();
		for (unsigned int i = 0; i < str.size(); i++)
		{
			std::string s({ str[i] });
			Renderer::DrawText(TextMap[s], 10 + 4 * i, 15);
		}

		Renderer::DrawText(clients_text, 14, 20);
		ss.str("");
		ss << lobby_count;
		str = ss.str();
		for (unsigned int i = 0; i < str.size(); i++)
		{
			std::string s({ str[i] });
			Renderer::DrawText(TextMap[s], 80 + 4 * i, 20);
		}
	}
	else if (state == Join)
	{
		unsigned char *ip = Data::JoinData.IP;
		unsigned short &port = Data::JoinData.Port;
		unsigned int index = Data::JoinData.Index;

		std::ostringstream ss;
		ss.fill('0');
		ss << std::setw(3) << (unsigned short)ip[0] << "."
			<< std::setw(3) << (unsigned short)ip[1] << "."
			<< std::setw(3) << (unsigned short)ip[2] << "."
			<< std::setw(3) << (unsigned short)ip[3] << ":"
			<< std::setw(5) << port;
		std::string disp = ss.str();
		for (unsigned int i = 0; i < disp.size(); i++)
		{
			std::string s({ disp[i] });
			Renderer::DrawText(TextMap[s], 10 + 4 * i, 15);
		}
		int iPos = index;
		if (index > 2) iPos++;
		if (index > 5) iPos++;
		if (index > 8) iPos++;
		if (index > 11) iPos++;
		Renderer::DrawText(TextMap["^"], 10 + 4 * iPos, 20);
	}
	else if (state == ClientWaiting)
	{
		// TODO
	}
	else if (state == ClientConnected)
	{
		// TODO
	}
	else if (state == Gameplay)
	{
		Game *game = Data::GameplayData.Local;
		Sprite &sprite = Data::GameplayData.PacSprite;
		unsigned int &frame = Data::GameplayData.AnimFrame;

		for (unsigned int i = 0; i < game->Players.size(); i++)
		{
			Renderer::DrawSprite(
					sprite,
					game->Players[i].XPos, game->Players[i].YPos,
					game->Players[i].CurrentDir * -90.f,
					0, frame++);
		}
	}
	else if (state == Exiting)
	{ }
	Renderer::Display();
}

static void change(MainState &state, MainState nextState)
{
	state = nextState;
	if (nextState == MainMenu)
	{
		NetworkManager::ResetConnections();
	}
	else if (nextState == Host)
	{
		Data::HostData.PlayerCount = 0;
	}
	else if (nextState == Join)
	{ }
	else if (nextState == ClientWaiting)
	{ }
	else if (nextState == ClientConnected)
	{ }
	else if (nextState == Gameplay)
	{
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

		unsigned int count = Data::GameplayData.PlayerCount;
		Data::GameplayData.PlayerNumber = Data::ClientConnectedData.PlayerNumber;
		Data::GameplayData.PlayerInputs =
			std::vector<std::vector<Player::Direction> >(
				count,
				std::vector<Player::Direction>(
					InputData_size, Player::Right));
		Data::GameplayData.ReceivedFrames = std::vector<unsigned short>(count);
		std::vector<Player> p(count, Player(Player::Pacman));
		Data::GameplayData.Local = new Game(f, p);
		Data::GameplayData.Synced = new Game(f, p);
		// TODO: delete these
	}
	else if (nextState == Exiting)
	{ }
}
