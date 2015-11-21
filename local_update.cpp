#include "local_update.h"
#include "Data.h"
#include "InputHandler.h"
#include "NetworkManager.h"
#include "MenuItem.h"
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
		std::vector<char> data_s(ConfirmClient_size, 0);
		data_s[ConfirmClient_PlayerNumber] = i;
		NetworkManager::Send(NetworkManager::ConfirmClient, data_s, i);
	}

	if (InputHandler::InputTime == 0)
	{
		if (InputHandler::LastInput == Player::Right)
		{
			std::vector<char> data_s(StartGame_size, 0);
			data_s[StartGame_PlayerCount] = (char)lobby_count;
			NetworkManager::Broadcast(NetworkManager::StartGame, data_s);
		}
		else if (InputHandler::LastInput == Player::Left)
		{
			return MainMenu;
		}
	}
	else
	{
		std::vector<char> data_s(PingClient_size, 0);
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
		std::vector<char> data_s(RequestServer_size, 0);
		NetworkManager::Send(NetworkManager::RequestServer, data_s, 0);
	}

	return state;
}

MainState local_client_connected(MainState state)
{
	if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
	{
		return Join;
	}

	// ping server
	std::vector<char> data_s(PingServer_size, 0);
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

	if ((game->CurrentFrame - sync->CurrentFrame + 1) * 2 < InputData_size)
	{
		game->Players[playerNumber].NextDir = InputHandler::LastInput;
		PlayerInputs[playerNumber].erase(PlayerInputs[playerNumber].begin());
		PlayerInputs[playerNumber].push_back(InputHandler::LastInput);
		game->update();
	}

	unsigned short all_received = *std::min_element(
			std::begin(ReceivedFrames), std::end(ReceivedFrames));
	while (sync->CurrentFrame < all_received)
	{
		for (unsigned int i = 0; i < ReceivedFrames.size(); i++)
		{
			sync->Players[i].NextDir = PlayerInputs[i][
				InputData_size - 1
					+ sync->CurrentFrame - game->CurrentFrame];
		}
		sync->update();
	}

	int currentFrame = game->CurrentFrame;
	*game = *sync;
	while (game->CurrentFrame < currentFrame)
	{
		game->Players[playerNumber].NextDir = PlayerInputs[playerNumber][
			InputData_size - 1 + game->CurrentFrame - currentFrame];
		game->update();
	}

	std::vector<char> data_s(OwnInputs_size, 0);
	int f = game->CurrentFrame;
	ReceivedFrames[playerNumber] = f;
	for (int i = OwnInputs_Frame + Frame_size - 1;
			i >= OwnInputs_Frame; i--)
	{
		data_s[i] = (char)(f & 0xFF);
		f = f >> 8;
	}
	for (unsigned int i = 0; i < InputData_size; i++)
	{
		data_s[OwnInputs_InputData + i] = PlayerInputs[playerNumber][i];
	}
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
		unsigned short &port = Data::JoinData.Port;
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
