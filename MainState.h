//! \file
//! MainState class declaration

#pragma once

#include "MainStateEnum.h"
#include "InputHandler.h"
#include "NetworkManager.h"
#include "Renderer.h"
#include "MenuItem.h"
#include "Player.h"
#include "Game.h"

class MainState
{
	public:
		MainStateEnum ProcessData();
		virtual MainStateEnum GetEnum() = 0;
		virtual MainStateEnum LocalUpdate() { return GetEnum(); };
		virtual MainStateEnum ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id)
		{ return GetEnum(); };
		virtual void Render() const { };
};

class MainMenuState : public MainState
{
	public:
		unsigned int Index;
		Menu MenuItems;
		std::vector<std::string> Menu_EN;

		MainStateEnum GetEnum() { return MainMenu; }
		MainStateEnum LocalUpdate();
		void Render() const;
};

class JoinState : public MainState
{
	public:
		unsigned int Index;
		unsigned char IP[4];
		unsigned short Port;

		MainStateEnum GetEnum() { return Join; }
		MainStateEnum LocalUpdate();
		void Render() const;
		void AddrIncrement(int amount);
};

class HostLobbyState : public MainState
{
	public:
		unsigned int PlayerCount;
		std::vector<bool> PlayersReady;
		std::vector<Character> Characters;

		MainStateEnum GetEnum() { return HostLobby; }
		MainStateEnum LocalUpdate();
		MainStateEnum ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class HostGameplayState : public MainState
{
	public:
		unsigned int PlayerCount;
		std::vector<Character> Characters;

		MainStateEnum GetEnum() { return HostGameplay; }
		MainStateEnum LocalUpdate() { return HostGameplay; }
		MainStateEnum ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class ClientWaitingState : public MainState
{
	public:
		unsigned int PlayerNumber;

		MainStateEnum GetEnum() { return ClientWaiting; }
		MainStateEnum LocalUpdate();
		MainStateEnum ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class ClientConnectedState : public MainState
{
	public:
		unsigned int Index;
		unsigned int PlayerNumber;
		Character SelectedCharacter;
		bool Ready;
		Game *StartingGame;
		Sprite PacmanSprite;
		std::vector<Sprite> GhostSprites;

		MainStateEnum GetEnum() { return ClientConnected; }
		MainStateEnum LocalUpdate();
		MainStateEnum ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class GameplayState : public MainState
{
	public:
		Game *Local;
		Game *Synced;
		std::vector<std::vector<Position> > PlayerInputs;
		std::vector<unsigned short> ReceivedFrames;
		unsigned int PlayerNumber;

		MainStateEnum GetEnum() { return Gameplay; }
		MainStateEnum LocalUpdate();
		MainStateEnum ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class ExitingState : public MainState
{
	public:
		MainStateEnum GetEnum() { return Exiting; }
		MainStateEnum LocalUpdate()
		{
			InputHandler::WindowClosed = true;
			return Exiting;
		}
};
