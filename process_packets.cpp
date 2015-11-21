#include "process_packets.h"
#include "Data.h"
#include "NetworkManager.h"

MainState process_data(MainState state)
{
	NetworkManager::LagIncrement();
	NetworkManager::MessageType mtype;
	std::vector<char> data_r;
	unsigned int id;
	NetworkManager::Receive(mtype, data_r, id);
	while (mtype != NetworkManager::None)
	{
		MainState s = process_packet(state, mtype, data_r, id);
		if (s != state)
		{
			return s;
		}
		NetworkManager::Receive(mtype, data_r, id);
	}

	return state;
}

MainState process_packet(MainState state, NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	if (state == Host)
	{
		return process_host(mtype, data_r, id);
	}
	else if (state == ClientWaiting)
	{
		return process_client_waiting(mtype, data_r, id);
	}
	else if (state == ClientConnected)
	{
		return process_client_connected(mtype, data_r, id);
	}
	else if (state == Gameplay)
	{
		return process_gameplay(mtype, data_r, id);
	}

	return state;
}

MainState process_host(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	unsigned int &lobby_count = Data::HostData.PlayerCount;

	if (id < lobby_count)
	{
		NetworkManager::CurrentConnections[id].Lag = 0;
	}
	if (mtype == NetworkManager::RequestServer)
	{
		// check if client is already in lobby
		if (id == lobby_count)
		{
			lobby_count++;
		}
		NetworkManager::CurrentConnections.resize(lobby_count);
		std::vector<char> data_s(ConfirmClient_size);
		data_s[ConfirmClient_PlayerNumber] = id;
		NetworkManager::Send(NetworkManager::ConfirmClient, data_s, id);
	}
	else if (mtype == NetworkManager::PingServer)
	{
		// No action needed
	}
	else if (mtype == NetworkManager::DisconnectServer)
	{
		NetworkManager::CurrentConnections.erase(
				NetworkManager::CurrentConnections.begin() + id);
		lobby_count--;
		// TODO renumber clients
	}
	else if (mtype == NetworkManager::OwnInputs)
	{
		std::vector<char> data_s(OtherInputs_size, 0);
		data_s[OtherInputs_PlayerNumber] = id;
		for (unsigned int i = 0; i < Frame_size; i++)
		{
			data_s[OtherInputs_Frame + i] = data_r[OwnInputs_Frame + i];
		}
		for (unsigned int i = 0; i < InputData_size; i++)
		{
			data_s[OtherInputs_InputData + i] =
				data_r[OwnInputs_InputData + i];
		}
		NetworkManager::Broadcast(NetworkManager::OtherInputs, data_s);
	}
	if (id >= lobby_count)
	{
		NetworkManager::CurrentConnections.resize(lobby_count);
	}

	return Host;
}

MainState process_client_waiting(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	if (id == 0)
	{
		if (mtype == NetworkManager::ConfirmClient)
		{
			// TODO: display confirmation
			Data::ClientConnectedData.PlayerNumber = data_r[ConfirmClient_PlayerNumber];
			return ClientConnected;
		}
	}

	return ClientWaiting;
}

MainState process_client_connected(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	if (id == 0)
	{
		NetworkManager::CurrentConnections[0].Lag = 0;
		if (mtype == NetworkManager::PingClient)
		{
			// No action needed
		}
		else if (mtype == NetworkManager::ConfirmClient)
		{
			Data::ClientConnectedData.PlayerNumber =
				data_r[ConfirmClient_PlayerNumber];
		}
		else if (mtype == NetworkManager::DisconnectClient)
		{
			return Join;
		}
		else if (mtype == NetworkManager::StartGame)
		{
			unsigned char count = data_r[StartGame_PlayerCount];
			Data::GameplayData.PlayerCount = count;
			return Gameplay;
		}
	}

	return ClientConnected;
}

MainState process_gameplay(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	Game *game = Data::GameplayData.Local;
	unsigned int playerNumber = Data::GameplayData.PlayerNumber;
	std::vector<std::vector<Player::Direction> > &PlayerInputs =
		Data::GameplayData.PlayerInputs;
	std::vector<unsigned short> &ReceivedFrames =
		Data::GameplayData.ReceivedFrames;

	if (id == 0)
	{
		if (mtype == NetworkManager::OtherInputs)
		{
			unsigned int num = data_r[OtherInputs_PlayerNumber];
			unsigned short f = 0;
			for (unsigned int i = 0; i < Frame_size; i++)
			{
				f = f << 8;
				f += (unsigned char)data_r[OtherInputs_Frame + i];
			}
			// TODO Do something smart if we receive something from the future
			int difference = game->CurrentFrame - f;
			if (num != playerNumber && f > ReceivedFrames[num])
			{
				for (unsigned int i = std::max(0, -difference),
					end = std::min(InputData_size, InputData_size - difference);
						i < end; i++)
				{
					Player::Direction input =
						(Player::Direction)
						data_r[OtherInputs_InputData + difference + i];
					PlayerInputs[num][i] = input;
				}
				ReceivedFrames[num] = f;
			}
		}
	}

	return Gameplay;
}
