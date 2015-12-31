//! \file
//! MainState class declaration

#pragma once

#include "InputHandler.h"
#include "NetworkManager.h"
#include "Renderer.h"
#include "Menu.h"
#include "Player.h"
#include "Game.h"

class MainState
{
	public:
		void ProcessData();
		virtual void Change() { }
		virtual void LocalUpdate() { }
		virtual void ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id) { }
		virtual void Render() const { }
};

class MainMenuState : public MainState
{
	public:
		static Menu MenuItems;

		static void Init();
		virtual void Change();
		void LocalUpdate();
		void Render() const;
};

class JoinState : public MainState
{
	public:
		static unsigned int Index;
		static unsigned char IP[4];
		static unsigned short Port;

		static void Init();
		void LocalUpdate();
		void Render() const;
		void AddrIncrement(int amount);
};

class HostLobbyState : public MainState
{
	public:
		unsigned int Field;
		unsigned int PlayerCount;
		std::vector<bool> PlayersReady;
		std::vector<Character> Characters;
		Menu MenuItems;

		HostLobbyState();
		void Change();
		void LocalUpdate();
		void ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
		static void StartGame(HostLobbyState *state);
		static void CloseLobby(void *);
};

class HostGameplayState : public MainState
{
	public:
		unsigned int PlayerCount;
		std::vector<Character> Characters;
		std::vector<bool> GameEnded;

		HostGameplayState(
				unsigned int playerCount,
				const std::vector<Character> &characters);
		void ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class ClientWaitingState : public MainState
{
	public:
		static unsigned int PlayerNumber;

		void LocalUpdate();
		void ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class ClientConnectedState : public MainState
{
	public:
		static unsigned int Index;
		static unsigned int PlayerNumber;
		static Character SelectedCharacter;
		static bool Ready;
		static Game *StartingGame;
		static Sprite PacmanSprite;
		static std::vector<Sprite> GhostSprites;
		static Menu MenuItems;

		static void Init();
		void Change();
		void LocalUpdate();
		void ProcessPacket(NetworkManager::MessageType mtype,
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

		GameplayState(Game *startingGame, unsigned int playerNumber);
		void LocalUpdate();
		void ProcessPacket(NetworkManager::MessageType mtype,
				std::vector<char> &data_r, unsigned int id);
		void Render() const;
};

class ExitingState : public MainState
{
	public:
		void LocalUpdate() { InputHandler::WindowClosed = true; }
};
