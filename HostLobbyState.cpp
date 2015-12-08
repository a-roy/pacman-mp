//! \file
//! HostLobbyState class implementation

#include "MainState.h"
#include <sstream>

MainStateEnum HostLobbyState::LocalUpdate()
{
	unsigned int renumber = PlayerCount;

	for (unsigned int i = 0; i < PlayerCount; i++)
	{
		if (NetworkManager::CurrentConnections[i].Lag > NetworkTimeout)
		{
			NetworkManager::CurrentConnections.erase(
					NetworkManager::CurrentConnections.begin() + i);
			PlayerCount--;
			if (i < renumber)
			{
				renumber = i;
			}
			i--;
		}
	}

	for (unsigned int i = renumber; i < PlayerCount; i++)
	{
		std::vector<char> data_s(ConfirmClient_size);
		data_s[ConfirmClient_PlayerNumber] = i;
		NetworkManager::Send(NetworkManager::ConfirmClient, data_s, i);
	}

	if (InputHandler::InputTime == 0)
	{
		if (InputHandler::LastInput == Right)
		{
			bool ready = PlayerCount > 0;
			for (unsigned int i = 0; i < PlayerCount; i++)
			{
				if (!PlayersReady[i])
				{
					ready = false;
					break;
				}
			}
			if (ready)
			{
				std::vector<char> data_s(
						StartGame_minsize + PlayerCount);
				data_s[StartGame_PlayerCount] = (char)PlayerCount;
				// TODO add field selection
				data_s[StartGame_Field] = 0;
				std::copy(Characters.begin(),
						Characters.end(),
						&data_s[StartGame_Character]);
				NetworkManager::Broadcast(
						NetworkManager::StartGame, data_s);
				return HostGameplay;
			}
		}
		else if (InputHandler::LastInput == Left)
		{
			return MainMenu;
		}
	}
	else
	{
		std::vector<char> data_s(PingClient_size);
		NetworkManager::Broadcast(NetworkManager::PingClient, data_s);
	}
	return HostLobby;
}

MainStateEnum HostLobbyState::ProcessPacket(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	if (id < PlayerCount)
	{
		NetworkManager::CurrentConnections[id].Lag = 0;
	}
	if (mtype == NetworkManager::RequestServer)
	{
		// check if client is already in lobby
		if (id == PlayerCount)
		{
			PlayerCount++;
		}
		NetworkManager::CurrentConnections.resize(PlayerCount);
		Characters.resize(PlayerCount);
		PlayersReady.resize(PlayerCount);
		std::vector<char> data_s(ConfirmClient_size);
		data_s[ConfirmClient_PlayerNumber] = id;
		NetworkManager::Send(NetworkManager::ConfirmClient, data_s, id);
	}
	else if (mtype == NetworkManager::PingServer)
	{
		// No action needed
	}
	else if (mtype == NetworkManager::PlayerReady)
	{
		Characters[id] =
			static_cast<Character>(data_r[PlayerReady_Character]);
		PlayersReady[id] = true;
	}
	else if (mtype == NetworkManager::PlayerNotReady)
	{
		PlayersReady[id] = false;
	}
	else if (mtype == NetworkManager::DisconnectServer)
	{
		NetworkManager::CurrentConnections.erase(
				NetworkManager::CurrentConnections.begin() + id);
		Characters.erase(
				Characters.begin() + id);
		PlayersReady.erase(
				PlayersReady.begin() + id);
		PlayerCount--;
		for (unsigned int i = id; i < PlayerCount; i++)
		{
			std::vector<char> data_s(ConfirmClient_size);
			data_s[ConfirmClient_PlayerNumber] = id;
			NetworkManager::Send(NetworkManager::ConfirmClient, data_s, id);
		}
	}

	if (id >= PlayerCount)
	{
		NetworkManager::CurrentConnections.resize(PlayerCount);
		Characters.resize(PlayerCount);
		PlayersReady.resize(PlayerCount);
	}
	return HostLobby;
}

void HostLobbyState::Render() const
{
	std::string address = NetworkManager::GetAddress();
	unsigned short port = NetworkManager::GetPort();
	std::ostringstream ss;
	ss << address << ":" << port;
	std::string str = ss.str();
	Renderer::DrawText(0, str, 24, 60, 100);

	ss.str("");
	ss << "Clients connected: " << PlayerCount;
	str = ss.str();
	Renderer::DrawText(0, str, 24, 60, 140);

	ss.str("");
	ss << "Field " << 0;
	str = ss.str();
	Renderer::DrawText(0, str, 24, 60, 180);

	for (int i = 0; i < PlayerCount; i++)
	{
		ss.str("");
		ss << NetworkManager::CurrentConnections[i].Address
			<< ":"
			<< NetworkManager::CurrentConnections[i].Port
			<< (PlayersReady[i] ? " Ready" : " Not ready");
		str = ss.str();
		Renderer::DrawText(0, str, 18, 60, 220 + 40 * i);
	}

	ss.str("");
	ss << "Start Game >";
	str = ss.str();
	Renderer::DrawText(0, str, 24, 60, 420);
}
