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

static void change(MainState *&currentState, MainStateEnum nextState);

std::vector<MainState *> states;

int main()
{
	Renderer::LoadFont("../prstartk.ttf");
	Renderer::TileScale = 3.0f;
	Renderer::SpriteScale = 3.0f;
	Renderer::CreateWindow(768, 768, "My window");

	states = std::vector<MainState *>(MAINSTATEENUM_NUMITEMS);
	states[MainMenu] = new MainMenuState();
	states[Host] = new HostState();
	states[Join] = new JoinState();
	states[ClientWaiting] = new ClientWaitingState();
	states[ClientConnected] = new ClientConnectedState();
	states[Gameplay] = new GameplayState();
	states[Exiting] = new ExitingState();

	MainMenuState *State_MainMenu = (MainMenuState *)states[MainMenu];
	HostState *State_Host = (HostState *)states[Host];
	JoinState *State_Join = (JoinState *)states[Join];
	ClientWaitingState *State_ClientWaiting =
		(ClientWaitingState *)states[ClientWaiting];
	ClientConnectedState *State_ClientConnected =
		(ClientConnectedState *)states[ClientConnected];
	GameplayState *State_Gameplay = (GameplayState *)states[Gameplay];

	// Initialize state data
	State_MainMenu->Index = 0;

	State_MainMenu->Menu_EN.push_back("Host game");
	State_MainMenu->Menu_EN.push_back("Join game");
	State_MainMenu->Menu_EN.push_back("Quit");

	MenuItem item1, item2, item3;
	item1.Text = 0;
	item1.Function = Host;
	State_MainMenu->MenuItems.push_back(item1);
	item2.Text = 1;
	item2.Function = Join;
	State_MainMenu->MenuItems.push_back(item2);
	item3.Text = 2;
	item3.Function = Exiting;
	State_MainMenu->MenuItems.push_back(item3);

	State_Join->Index = 0;
	std::string myAddress = NetworkManager::GetAddress();
	std::stringstream addr(myAddress);
	std::string part;
	for (unsigned int i = 0; i < 4 && std::getline(addr, part, '.'); i++)
	{
		std::stringstream num(part);
		unsigned short n;
		num >> n;
		State_Join->IP[i] = n;
	}
	State_Join->Port = 0;

	State_ClientConnected->Index = 0;

	// Initialize sprites
	Pacman::PacmanSprite.Index =
		Renderer::CreateSprite("../spritesheet.png");
	Ghost::GhostSprite.Index =
		Renderer::CreateSprite("../spritesheet.png");

	Animation pac_move(4);
	pac_move.AddFrame(1, 1, 16, 16);
	pac_move.AddFrame(18, 1, 16, 16);
	pac_move.AddFrame(35, 1, 16, 16);
	pac_move.AddFrame(18, 1, 16, 16);
	Pacman::PacmanSprite.Animations.push_back(pac_move);
	Animation pac_die(4);
	for (int i = 3; i <= 15; i++)
	{
		pac_die.AddFrame(1 + 17 * i, 1, 16, 16);
	}
	Pacman::PacmanSprite.Animations.push_back(pac_die);

	Animation gho_hori(8);
	gho_hori.AddFrame(1, 18, 16, 16);
	gho_hori.AddFrame(18, 18, 16, 16);
	Ghost::GhostSprite.Animations.push_back(gho_hori);
	Animation gho_up(8);
	gho_up.AddFrame(35, 18, 16, 16);
	gho_up.AddFrame(52, 18, 16, 16);
	Ghost::GhostSprite.Animations.push_back(gho_up);
	Animation gho_down(8);
	gho_down.AddFrame(69, 18, 16, 16);
	gho_down.AddFrame(86, 18, 16, 16);
	Ghost::GhostSprite.Animations.push_back(gho_down);
	Animation gho_fear(8);
	gho_fear.AddFrame(205, 18, 16, 16);
	gho_fear.AddFrame(222, 18, 16, 16);
	Ghost::GhostSprite.Animations.push_back(gho_fear);

	// Initialize NetworkManager
	NetworkManager::Init();

	// Start the state machine
	MainState* currentState = states[MainMenu];

    while (Renderer::WindowOpen())
    {
		InputHandler::PollEvents();
		if (InputHandler::WindowClosed)
		{
			break;
		}

		change(currentState, currentState->ProcessData());
		change(currentState, currentState->LocalUpdate());
		Renderer::Clear();
		currentState->Render();
		Renderer::Display();
    }

	Renderer::Deinit();
	return 0;
}

static void change(MainState *&currentState, MainStateEnum nextState)
{
	if (currentState == states[nextState])
	{
		return;
	}

	if (nextState == Host)
	{
		HostState *host = (HostState *)states[Host];
		host->PlayerCount = 0;
	}
	else if (nextState == ClientConnected)
	{
		ClientWaitingState *waiting =
			(ClientWaitingState *)states[ClientWaiting];
		ClientConnectedState *connected =
			(ClientConnectedState *)states[ClientConnected];
		connected->PlayerNumber = waiting->PlayerNumber;
		connected->SelectedCharacter = Pacman_c;
		connected->Ready = false;
		connected->StartingGame = NULL;
	}
	else if (nextState == Gameplay)
	{
		ClientConnectedState *client =
			(ClientConnectedState *)states[ClientConnected];
		GameplayState *gameplay = (GameplayState *)states[Gameplay];
		gameplay->Local = client->StartingGame;
		gameplay->Synced = new Game(*gameplay->Local);
		gameplay->PlayerNumber = client->PlayerNumber;
		unsigned int count = gameplay->Local->Players.size();
		gameplay->PlayerInputs = std::vector<std::vector<Position> >(
				count, std::vector<Position>(InputData_size, Left));
		gameplay->ReceivedFrames = std::vector<unsigned short>(count);
	}

	currentState = states[nextState];
}
