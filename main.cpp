//! \file
//! Entry point

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "MainState.h"
#include "StateMachine.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "NetworkManager.h"

int main()
{
	Renderer::LoadFont("../prstartk.ttf");
	Renderer::TileScale = 3.0f;
	Renderer::SpriteScale = 3.0f;
	Renderer::CreateWindow(768, 768, "My window");

	// Initialize state data
	MainMenuState::MenuItems.AddItem(
			new FunctionalMenuItem("Host game",
			[](){ StateMachine::Change(new HostLobbyState()); },
			[](){ }));
	MainMenuState::MenuItems.AddItem(
			new FunctionalMenuItem("Join game",
			[](){ StateMachine::Change(new JoinState()); },
			[](){ }));
	MainMenuState::MenuItems.AddItem(
			new FunctionalMenuItem("Quit",
			[](){ StateMachine::Change(new ExitingState()); },
			[](){ }));

	JoinState::Index = 0;
	std::string myAddress = NetworkManager::GetAddress();
	std::stringstream addr(myAddress);
	std::string part;
	for (unsigned int i = 0; i < 4 && std::getline(addr, part, '.'); i++)
	{
		std::stringstream num(part);
		unsigned short n;
		num >> n;
		JoinState::IP[i] = n;
	}
	JoinState::Port = 0;

	HostLobbyState::MenuItems.AddItem(
			new FieldMenuItem(&HostLobbyState::Field));
	HostLobbyState::MenuItems.AddItem(
			new FunctionalMenuItem(
				"  Start Game >", &HostLobbyState::StartGame, [](){ }));
	HostLobbyState::MenuItems.AddItem(
			new FunctionalMenuItem(
				"< Close Lobby", [](){ }, &HostLobbyState::CloseLobby));

	ClientConnectedState::MenuItems.AddItem(
			new CharacterMenuItem(&ClientConnectedState::SelectedCharacter));
	ClientConnectedState::MenuItems.AddItem(
			new ReadyMenuItem(&ClientConnectedState::Ready));

	// Initialize sprites
	ClientConnectedState::PacmanSprite.Index =
		Renderer::CreateSprite("../spritesheet.png");

	Animation pac_move(4);
	pac_move.AddFrame(1, 1, 16, 16);
	pac_move.AddFrame(18, 1, 16, 16);
	pac_move.AddFrame(35, 1, 16, 16);
	pac_move.AddFrame(18, 1, 16, 16);
	ClientConnectedState::PacmanSprite.Animations.push_back(pac_move);
	Animation pac_die(4);
	for (int i = 3; i <= 15; i++)
	{
		pac_die.AddFrame(1 + 17 * i, 1, 16, 16);
	}
	ClientConnectedState::PacmanSprite.Animations.push_back(pac_die);

	ClientConnectedState::GhostSprites.resize(4);
	for (unsigned int i = 0; i < 4; i++)
	{
		ClientConnectedState::GhostSprites[i].Index =
			Renderer::CreateSprite("../spritesheet.png");
		Animation gho_hori(8);
		gho_hori.AddFrame(1, 18 + 17 * i, 16, 16);
		gho_hori.AddFrame(18, 18 + 17 * i, 16, 16);
		ClientConnectedState::GhostSprites[i].Animations.push_back(gho_hori);
		Animation gho_up(8);
		gho_up.AddFrame(35, 18 + 17 * i, 16, 16);
		gho_up.AddFrame(52, 18 + 17 * i, 16, 16);
		ClientConnectedState::GhostSprites[i].Animations.push_back(gho_up);
		Animation gho_down(8);
		gho_down.AddFrame(69, 18 + 17 * i, 16, 16);
		gho_down.AddFrame(86, 18 + 17 * i, 16, 16);
		ClientConnectedState::GhostSprites[i].Animations.push_back(gho_down);
		Animation gho_fear(8);
		gho_fear.AddFrame(205, 18 + 17 * i, 16, 16);
		gho_fear.AddFrame(222, 18 + 17 * i, 16, 16);
		ClientConnectedState::GhostSprites[i].Animations.push_back(gho_fear);
	}

	// Initialize NetworkManager
	NetworkManager::Init();

	StateMachine::Change(new MainMenuState());
    while (Renderer::WindowOpen())
    {
		InputHandler::PollEvents();
		if (InputHandler::WindowClosed)
		{
			break;
		}

		StateMachine::CurrentState->ProcessData();
		StateMachine::CurrentState->LocalUpdate();
		Renderer::Clear();
		StateMachine::CurrentState->Render();
		Renderer::Display();
    }

	Renderer::Deinit();
	return 0;
}
