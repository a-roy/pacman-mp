#pragma once

#include <map>
#include <string>
#include <vector>
#include "Game.h"
#include "MenuItem.h"
#include "Sprite.h"

class Data
{
	public:
		struct MainMenuData_t
		{
			unsigned int Index;
			std::vector<std::string> Menu_EN;
			Menu MenuItems;
		};

		struct HostData_t
		{
			unsigned int PlayerCount;
			std::vector<Character> Characters;
			std::vector<bool> PlayersReady;
		};

		struct JoinData_t
		{
			unsigned int Index;
			unsigned char IP[4];
			unsigned short Port;
		};

		struct ClientConnectedData_t
		{
			unsigned int PlayerNumber;
			unsigned int Index;
			Character SelectedCharacter;
			bool Ready;
		};

		struct GameplayData_t
		{
			Game *Local;
			Game *Synced;
			Sprite PacmanSprite;
			Sprite GhostSprite;
			unsigned int AnimFrame;
			unsigned int PlayerCount;
			unsigned int PlayerNumber;
			std::vector<std::vector<Direction> > PlayerInputs;
			std::vector<unsigned short> ReceivedFrames;
			std::vector<Character> Characters;
		};

		static int Font;
		static int ClientsText;
		static MainMenuData_t MainMenuData;
		static HostData_t HostData;
		static JoinData_t JoinData;
		static ClientConnectedData_t ClientConnectedData;
		static GameplayData_t GameplayData;
};
