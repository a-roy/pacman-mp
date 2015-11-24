#include "local_update.h"
#include "Data.h"
#include "InputHandler.h"
#include "NetworkManager.h"
#include "MenuItem.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>

MainState local_update(MainState state)
{
	if (state == MainMenu)
	{
		return local_main_menu(state);
	}
	else if (state == Join)
	{
		return local_join(state);
	}
	else if (state == Host)
	{
		return local_host(state);
	}
	else if (state == ClientWaiting)
	{
		return local_client_waiting(state);
	}
	else if (state == ClientConnected)
	{
		return local_client_connected(state);
	}
	else if (state == Gameplay)
	{
		return local_gameplay(state);
	}

	return state;
}

MainState local_main_menu(MainState state)
{
	unsigned int &index = Data::MainMenuData.Index;
	Menu &menu = Data::MainMenuData.MenuItems;

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
			return menu[index].Function;
		}
	}

	return state;
}

MainState local_join(MainState state)
{
	unsigned int &index = Data::JoinData.Index;

	if (InputHandler::InputTime == 0)
	{
		if (InputHandler::LastInput == Player::Left)
		{
			if (index > 0)
			{
				index--;
			}
			else
			{
				return MainMenu;
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
				unsigned char *ip = Data::JoinData.IP;
				ss << (unsigned short)ip[0] << "."
					<< (unsigned short)ip[1] << "."
					<< (unsigned short)ip[2] << "."
					<< (unsigned short)ip[3];
				NetworkManager::ResetConnections();
				NetworkManager::GetConnection(ss.str(), Data::JoinData.Port);
				return ClientWaiting;
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

	return state;
}

MainState local_host(MainState state)
{
	unsigned int &lobby_count = Data::HostData.PlayerCount;
	unsigned int renumber = lobby_count;

	for (unsigned int i = 0; i < lobby_count; i++)
	{
		if (NetworkManager::CurrentConnections[i].Lag > NetworkTimeout)
		{
			NetworkManager::CurrentConnections.erase(
					NetworkManager::CurrentConnections.begin() + i);
			lobby_count--;
			if (i < renumber)
			{
				renumber = i;
			}
			i--;
		}
	}

	for (unsigned int i = renumber; i < lobby_count; i++)
	{
		std::vector<char> data_s(ConfirmClient_size);
		data_s[ConfirmClient_PlayerNumber] = i;
		NetworkManager::Send(NetworkManager::ConfirmClient, data_s, i);
	}

	if (InputHandler::InputTime == 0)
	{
		if (InputHandler::LastInput == Player::Right)
		{
			std::vector<char> data_s(StartGame_minsize + lobby_count);
			data_s[StartGame_PlayerCount] = (char)lobby_count;
			// TODO add field selection
			data_s[StartGame_Field] = 0;
			std::copy(Data::HostData.Characters.begin(),
					Data::HostData.Characters.end(),
					&data_s[StartGame_Character]);
			NetworkManager::Broadcast(NetworkManager::StartGame, data_s);
		}
		else if (InputHandler::LastInput == Player::Left)
		{
			return MainMenu;
		}
	}
	else
	{
		std::vector<char> data_s(PingClient_size);
		NetworkManager::Broadcast(NetworkManager::PingClient, data_s);
	}

	return state;
}

MainState local_client_waiting(MainState state)
{
	if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
	{
		return Join;
	}
	else
	{
		std::vector<char> data_s(RequestServer_size);
		NetworkManager::Send(NetworkManager::RequestServer, data_s, 0);
	}

	return state;
}

MainState local_client_connected(MainState state)
{
	unsigned int &index = Data::ClientConnectedData.Index;
	Character &c = Data::ClientConnectedData.SelectedCharacter;
	if (InputHandler::InputTime == 0)
	{
		if (index == 0)
		{
			if (InputHandler::LastInput == Player::Down)
			{
				index = 1;
			}
			else if (InputHandler::LastInput == Player::Left
					|| InputHandler::LastInput == Player::Right)
			{
				if (c == Pacman_c)
				{
					c = Ghost_c;
				}
				else
				{
					c = Pacman_c;
				}
			}
		}
		else
		{
			bool &ready = Data::ClientConnectedData.Ready;
			if (InputHandler::LastInput == Player::Up)
			{
				ready = false;
				index = 0;
			}
			else if (InputHandler::LastInput == Player::Right)
			{
				ready = true;
				std::vector<char> data_s(PlayerReady_size);
				data_s[PlayerReady_Character] = c;
				NetworkManager::Send(NetworkManager::PlayerReady, data_s, 0);
			}
			else if (InputHandler::LastInput == Player::Left)
			{
				ready = false;
			}
		}
	}

	if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
	{
		return Join;
	}

	// ping server
	std::vector<char> data_s(PingServer_size);
	NetworkManager::Send(NetworkManager::PingServer, data_s, 0);

	return state;
}

MainState local_gameplay(MainState state)
{
	Game *game = Data::GameplayData.Local;
	Game *sync = Data::GameplayData.Synced;
	std::vector<std::vector<Player::Direction> > &PlayerInputs
		= Data::GameplayData.PlayerInputs;
	std::vector<unsigned short> &ReceivedFrames
		= Data::GameplayData.ReceivedFrames;
	unsigned int playerNumber = Data::GameplayData.PlayerNumber;

	// Instead of changing our frame number, we use the delay to read inputs
	// from previous frames
	if ((game->CurrentFrame - sync->CurrentFrame + 1) * 2 < InputData_size)
	{
		PlayerInputs[playerNumber].erase(PlayerInputs[playerNumber].begin());
		PlayerInputs[playerNumber].push_back(InputHandler::LastInput);
		game->Players[playerNumber]->NextDir =
			PlayerInputs[playerNumber][InputData_size - 1 - NetworkDelay];
		game->update();
	}

	unsigned short all_received = *std::min_element(
			std::begin(ReceivedFrames), std::end(ReceivedFrames));
	while (sync->CurrentFrame < all_received)
	{
		for (unsigned int i = 0; i < ReceivedFrames.size(); i++)
		{
			sync->Players[i]->NextDir = PlayerInputs[i][
				InputData_size - 1 - NetworkDelay
					+ sync->CurrentFrame - game->CurrentFrame];
		}
		sync->update();
	}

	int currentFrame = game->CurrentFrame;
	*game = *sync;
	while (game->CurrentFrame < currentFrame)
	{
		game->Players[playerNumber]->NextDir = PlayerInputs[playerNumber][
			InputData_size - 1 - NetworkDelay
				+ game->CurrentFrame - currentFrame];
		game->update();
	}

	std::vector<char> data_s(OwnInputs_size);
	int f = currentFrame;
	ReceivedFrames[playerNumber] = f;
	for (int i = OwnInputs_Frame + Frame_size - 1;
			i >= OwnInputs_Frame; i--)
	{
		data_s[i] = (char)(f & 0xFF);
		f = f >> 8;
	}
	std::copy(
			PlayerInputs[playerNumber].begin(),
			PlayerInputs[playerNumber].end(),
			&data_s[OwnInputs_InputData]);
	NetworkManager::Send(NetworkManager::OwnInputs, data_s, 0);

	return state;
}

void addrIncrement(unsigned int digit, int amount)
{
	int delta = amount;
	if (digit < 12)
	{
		for (unsigned int i = digit % 3; i < 2; i++)
		{
			delta *= 10;
		}
		unsigned char &selected = Data::JoinData.IP[digit / 3];
		if (selected + delta == (selected + delta + 255) % 255)
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
		unsigned short &port = Data::JoinData.Port;
		if (port + delta == (port + delta + 65535) % 65535)
		{
			port += delta;
		}
		else
		{
			port = (amount > 0) ? 65535 : 0;
		}
	}
}
