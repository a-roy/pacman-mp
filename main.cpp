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

	Data::Font = Renderer::LoadFont("../prstartk.ttf");

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

	Renderer::TileScale = 6.0f;
	Renderer::SpriteScale = 3.0f;
	Renderer::CreateWindow(768, 768, "My window");

	Data::GameplayData.PacmanSprite.Index =
		Renderer::CreateSprite("../spritesheet.png");
	Data::GameplayData.GhostSprite.Index =
		Renderer::CreateSprite("../spritesheet.png");

	Data::JoinData.IP[0] = 127;
	Data::JoinData.IP[1] = 0;
	Data::JoinData.IP[2] = 0;
	Data::JoinData.IP[3] = 1;
	Data::JoinData.Port = 0;

	Animation pac_move(4);
	//pac_move.AddFrame(34, 170, 32, 32);
	//pac_move.AddFrame(1, 170, 32, 32);
	//pac_move.AddFrame(34, 170, 32, 32);
	//pac_move.AddFrame(67, 170, 32, 32);
	pac_move.AddFrame(18, 1, 16, 16);
	pac_move.AddFrame(35, 1, 16, 16);
	pac_move.AddFrame(18, 1, 16, 16);
	pac_move.AddFrame(1, 1, 16, 16);
	Data::GameplayData.PacmanSprite.Animations.push_back(pac_move);
	Data::GameplayData.AnimFrame = 0;

	//Animation gho_move(4);
	//gho_move.AddFrame(196, 105, 32, 32);
	//gho_move.AddFrame(196, 105, 32, 32);
	//gho_move.AddFrame(229, 105, 32, 32);
	//gho_move.AddFrame(229, 105, 32, 32);
	Animation gho_hori(4);
	gho_hori.AddFrame(1, 18, 16, 16);
	gho_hori.AddFrame(1, 18, 16, 16);
	gho_hori.AddFrame(18, 18, 16, 16);
	gho_hori.AddFrame(18, 18, 16, 16);
	Data::GameplayData.GhostSprite.Animations.push_back(gho_hori);
	Animation gho_up(4);
	gho_up.AddFrame(35, 18, 16, 16);
	gho_up.AddFrame(35, 18, 16, 16);
	gho_up.AddFrame(52, 18, 16, 16);
	gho_up.AddFrame(52, 18, 16, 16);
	Data::GameplayData.GhostSprite.Animations.push_back(gho_up);
	Animation gho_down(4);
	gho_down.AddFrame(69, 18, 16, 16);
	gho_down.AddFrame(69, 18, 16, 16);
	gho_down.AddFrame(86, 18, 16, 16);
	gho_down.AddFrame(86, 18, 16, 16);
	Data::GameplayData.GhostSprite.Animations.push_back(gho_down);
	//Data::GameplayData.GhostSprite.Animations.push_back(gho_move);

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

	if (state == MainMenu)
	{
		std::vector<std::string> &Menu_EN = Data::MainMenuData.Menu_EN;
		Menu &menu = Data::MainMenuData.MenuItems;
		unsigned int index = Data::MainMenuData.Index;

		for (unsigned int i = 0; i < menu.size(); i++)
		{
			Renderer::DrawText(
					Data::Font, Menu_EN[menu[i].Text], 24, 60, 100 + 40 * i);
			if (i == index)
			{
				Renderer::DrawText(Data::Font, ">", 24, 20, 100 + 40 * i);
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
		Renderer::DrawText(Data::Font, str, 24, 60, 100);

		ss.str("");
		ss << "Clients connected: " << lobby_count;
		str = ss.str();
		Renderer::DrawText(Data::Font, str, 24, 60, 140);

		ss.str("");
		ss << "Field " << 0;
		str = ss.str();
		Renderer::DrawText(Data::Font, str, 24, 60, 180);

		for (int i = 0; i < lobby_count; i++)
		{
			ss.str("");
			ss << NetworkManager::CurrentConnections[i].Address
				<< ":"
				<< NetworkManager::CurrentConnections[i].Port
				<< (Data::HostData.PlayersReady[i] ? " Ready" : " Not ready");
			str = ss.str();
			Renderer::DrawText(Data::Font, str, 18, 60, 220 + 40 * i);
		}

		ss.str("");
		ss << "Start Game >";
		str = ss.str();
		Renderer::DrawText(Data::Font, str, 24, 60, 420);
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
		Renderer::DrawText(Data::Font, disp, 24, 60, 100);
		int iPos = index;
		if (index > 2) iPos++;
		if (index > 5) iPos++;
		if (index > 8) iPos++;
		if (index > 11) iPos++;
		Renderer::DrawText(Data::Font, "^", 24, 60 + 24 * iPos, 136);
	}
	else if (state == ClientWaiting)
	{
		Renderer::DrawText(Data::Font, "Connecting...", 24, 60, 100);
	}
	else if (state == ClientConnected)
	{
		std::ostringstream ss;
		ss << "You are Player ";
		ss << Data::ClientConnectedData.PlayerNumber + 1;
		std::string str = ss.str();
		Renderer::DrawText(Data::Font, str, 24, 60, 100);
		Character c = Data::ClientConnectedData.SelectedCharacter;
		bool ready = Data::ClientConnectedData.Ready;
		unsigned int index = Data::ClientConnectedData.Index;
		if (c == Pacman_c)
		{
			Renderer::DrawText(Data::Font, "< Pac-Man >", 18, 60, 140);
		}
		else
		{
			Renderer::DrawText(Data::Font, "<  Ghost  >", 18, 60, 140);
		}
		if (ready)
		{
			Renderer::DrawText(Data::Font, "< Ready!", 18, 60, 180);
		}
		else
		{
			Renderer::DrawText(Data::Font, "  Ready? >", 18, 60, 180);
		}
		Renderer::DrawText(Data::Font, ">", 18, 20, 140 + 40 * index);
	}
	else if (state == Gameplay)
	{
		Game *game = Data::GameplayData.Local;
		unsigned int &frame = Data::GameplayData.AnimFrame;

		for (unsigned int i = 0; i < game->Players.size(); i++)
		{
			game->Players[i]->Draw();
		}
		std::ostringstream ss;
		ss << "Local: " << game->CurrentFrame;
		Renderer::DrawText(Data::Font, ss.str(), 18, 20, 20);
		ss.str("");
		Game *sync = Data::GameplayData.Synced;
		ss << "Synced: " << sync->CurrentFrame;
		Renderer::DrawText(Data::Font, ss.str(), 18, 20, 60);
	}
	else if (state == Exiting) { }
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
	else if (nextState == Join) { }
	else if (nextState == ClientWaiting) { }
	else if (nextState == ClientConnected) { }
	else if (nextState == Gameplay)
	{
		Field f("../stage1.txt");

		unsigned int count = Data::GameplayData.PlayerCount;
		Data::GameplayData.PlayerNumber =
			Data::ClientConnectedData.PlayerNumber;
		Data::GameplayData.PlayerInputs =
			std::vector<std::vector<Direction> >(
				count,
				std::vector<Direction>(
					InputData_size, Right));
		Data::GameplayData.ReceivedFrames = std::vector<unsigned short>(count);
		std::vector<Player *> pl, ps;
		for (unsigned int i = 0; i < count; i++)
		{
			if (Data::GameplayData.Characters[i] == Pacman_c)
			{
				pl.push_back(new Pacman());
				ps.push_back(new Pacman());
			}
			else if (Data::GameplayData.Characters[i] == Ghost_c)
			{
				pl.push_back(new Ghost());
				ps.push_back(new Ghost());
			}
		}
		Data::GameplayData.Local = new Game(f, pl);
		Data::GameplayData.Synced = new Game(f, ps);
		// TODO: delete these
	}
	else if (nextState == Exiting) { }
}
