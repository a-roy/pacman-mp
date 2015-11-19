//! \file
//! Entry point

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
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
Game* sync;
Field f;
std::map<std::string, int> TextMap;
std::vector<std::string> Menu_EN;
unsigned char ip[4];
unsigned short port;
Sprite sprite;
int frame;
unsigned int lobby_count = 0;

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
	NetworkManager::LagIncrement();

	if (state == MainMenu)
	{
		if (InputHandler::InputTime == 0)
		{
			if (InputHandler::LastInput == Player::Up)
			{
				if (index > 0)
				{
					index--;
				}
			}
			else if (InputHandler::LastInput == Player::Down)
			{
				if (index < menu.size() - 1)
				{
					index++;
				}
			}
			else if (InputHandler::LastInput == Player::Right)
			{
				MainState newState = menu[index].Function;
				change(state, newState);
			}
		}
	}
	else if (state == Host)
	{
		// handle incoming network traffic
		NetworkManager::MessageType mtype;
		std::vector<char> data;
		unsigned int id;
		NetworkManager::Receive(mtype, data, id);
		if (mtype == NetworkManager::RequestServer)
		{
			// check if client is already in lobby
			if (id == lobby_count)
			{
				lobby_count++;
			}
			NetworkManager::CurrentConnections.resize(lobby_count);
			std::vector<char> d(1);
			d.push_back(id);
			NetworkManager::Send(NetworkManager::ConfirmClient, d, id);
		}
		else if (mtype == NetworkManager::PingServer)
		{
			if (id < lobby_count)
			{
				NetworkManager::CurrentConnections[id].Lag = 0;
			}
			else
			{
				NetworkManager::CurrentConnections.resize(lobby_count);
			}
		}
		else if (mtype == NetworkManager::DisconnectServer)
		{
			NetworkManager::CurrentConnections.erase(
					NetworkManager::CurrentConnections.begin() + id);
			lobby_count--;
		}

		// check timeouts
		for (unsigned int i = 0; i < lobby_count; i++)
		{
			if (NetworkManager::CurrentConnections[i].Lag > NetworkTimeout)
			{
				NetworkManager::CurrentConnections.erase(
						NetworkManager::CurrentConnections.begin() + i);
				lobby_count--;
				i--;
			}
		}

		if (InputHandler::InputTime == 0 && InputHandler::LastInput == Player::Right)
		{
			std::vector<char> d(0);
			NetworkManager::Broadcast(NetworkManager::StartGame, d);
		}
		else
		{
			std::vector<char> d(0);
			NetworkManager::Broadcast(NetworkManager::PingClient, d);
		}
	}
	else if (state == Join)
	{
		if (InputHandler::InputTime == 0)
		{
			if (InputHandler::LastInput == Player::Left)
			{
				if (index > 0)
				{
					index--;
				}
			}
			else if (InputHandler::LastInput == Player::Right)
			{
				if (index < 16)
				{
					index++;
				}
				else
				{
					std::ostringstream ss;
					ss << (unsigned short)ip[0] << "."
						<< (unsigned short)ip[1] << "."
						<< (unsigned short)ip[2] << "."
						<< (unsigned short)ip[3];
					NetworkManager::ResetConnections();
					NetworkManager::GetConnection(ss.str(), port);
					change(state, ClientWaiting);
					return;
				}
			}
			else if (InputHandler::LastInput == Player::Up)
			{
				addrIncrement(index, 1);
			}
			else if (InputHandler::LastInput == Player::Down)
			{
				addrIncrement(index, -1);
			}
		}
	}
	else if (state == ClientWaiting)
	{
		NetworkManager::MessageType mtype;
		std::vector<char> data;
		unsigned int sender;
		NetworkManager::Receive(mtype, data, sender);
		if (sender == 0)
		{
			if (mtype == NetworkManager::ConfirmClient)
			{
				// TODO: display confirmation
				change(state, ClientConnected);
				return;
			}
		}

		if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
		{
			change(state, Join);
			return;
		}
		else
		{
			std::vector<char> data(0);
			NetworkManager::Send(NetworkManager::RequestServer, data, 0);
		}
	}
	else if (state == ClientConnected)
	{
		NetworkManager::MessageType mtype;
		std::vector<char> data;
		unsigned int sender;
		NetworkManager::Receive(mtype, data, sender);
		if (sender == 0)
		{
			if (mtype == NetworkManager::PingClient)
			{
				NetworkManager::CurrentConnections[0].Lag = 0;
			}
			else if (mtype == NetworkManager::DisconnectClient)
			{
				change(state, Join);
				return;
			}
			else if (mtype == NetworkManager::StartGame)
			{
				change(state, Gameplay);
				return;
			}
		}

		if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
		{
			change(state, Join);
			return;
		}

		// ping server
		std::vector<char> d(0);
		NetworkManager::Send(NetworkManager::PingServer, d, 0);
	}
	else if (state == Gameplay)
	{
		game->Players[0].NextDir = InputHandler::LastInput;
		game->update();
	}
	else if (state == Exiting)
	{
		// Don't do anything
	}
}

static void render(const MainState &state)
{
	Renderer::Clear();

	if (state == MainMenu)
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
	else if (state == Host)
	{
		std::string address = NetworkManager::GetAddress();
		unsigned short port = NetworkManager::GetPort();
		std::ostringstream ss;
		ss << address << ":" << port;
		std::string str = ss.str();
		for (unsigned int i = 0; i < str.size(); i++)
		{
			std::string s({ str[i] });
			Renderer::DrawText(TextMap[s], 10 + 6 * i, 15);
		}
	}
	else if (state == Join)
	{
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
			Renderer::DrawText(TextMap[s], 10 + 6 * i, 15);
		}
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
		Renderer::DrawSprite(
				sprite,
				game->Players[0].XPos, game->Players[0].YPos,
				game->Players[0].CurrentDir * -90.f,
				0, frame++);
		Renderer::DrawText(text, 10, 2);
	}
	else if (state == Exiting)
	{
		// Don't do anything
	}
	Renderer::Display();
}

static void change(MainState &state, MainState nextState)
{
	state = nextState;
	if (nextState == MainMenu)
	{
		index = 0;
	}
	else if (nextState == Host)
	{
		lobby_count = 0;
	}
	else if (nextState == Join)
	{
		index = 0;
		ip[0] = 127;
		ip[1] = 0;
		ip[2] = 0;
		ip[3] = 1;
		port = 1;
	}
	else if (state == ClientWaiting)
	{
		// TODO
	}
	else if (state == ClientConnected)
	{
		// TODO
	}
	else if (nextState == Gameplay)
	{
		std::vector<Player> p(1, Player(Player::Pacman));
		game = new Game(f, p);
		// TODO: delete game;
	}
	else if (nextState == Exiting)
	{
		// Don't do anything
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
