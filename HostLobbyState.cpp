//! \file
//! HostLobbyState class implementation

#include "MainState.h"
#include "StateMachine.h"
#include <sstream>

HostLobbyState::HostLobbyState() : Field(0), PlayerCount(0)
{
	MenuItems.AddItem(new FieldMenuItem(&Field));
	MenuItems.AddItem(new FunctionalMenuItem<HostLobbyState>(
				"  Start Game >", &StartGame, [](HostLobbyState *){ }, this));
	MenuItems.AddItem(new FunctionalMenuItem<void>(
				"< Close Lobby", [](void *){ }, &CloseLobby));
}

void HostLobbyState::Change()
{
	PlayerCount = 0;
}

void HostLobbyState::LocalUpdate()
{
	unsigned int renumber = PlayerCount;

	for (std::size_t i = 0; i < PlayerCount; i++)
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

	MenuItems.Update();
	std::vector<char> data_s(PingClient_size);
	NetworkManager::Broadcast(NetworkManager::PingClient, data_s);
}

void HostLobbyState::ProcessPacket(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
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
		Characters[id] =
			static_cast<Character>(data_r[PingServer_Character]);
		PlayersReady[id] = (bool)data_r[PingServer_Ready];
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
}

void HostLobbyState::Render() const
{
	std::string address = NetworkManager::GetAddress();
	unsigned short port = NetworkManager::GetPort();
	std::ostringstream ss;
	ss << address << ":" << port;
	std::string str = ss.str();
	Renderer::DrawText(str, 24, 60, 100);

	ss.str("");
	ss << "Clients connected: " << PlayerCount;
	str = ss.str();
	Renderer::DrawText(str, 24, 60, 140);

	for (std::size_t i = 0; i < PlayerCount; i++)
	{
		ss.str("");
		ss << NetworkManager::CurrentConnections[i].Address
			<< ":"
			<< NetworkManager::CurrentConnections[i].Port
			<< (PlayersReady[i] ? " Ready" : " Not ready");
		str = ss.str();
		Renderer::DrawText(str, 18, 60, 180 + 40 * i);
	}
	MenuItems.Render(20, 380);
}

void HostLobbyState::StartGame(HostLobbyState *state)
{
	bool ready = state->PlayerCount > 0;
	for (std::size_t i = 0; i < state->PlayerCount; i++)
	{
		if (!state->PlayersReady[i])
		{
			ready = false;
			break;
		}
	}
	if (ready)
	{
		std::vector<char> data_s(
				StartGame_minsize + state->PlayerCount);
		data_s[StartGame_PlayerCount] = (char)state->PlayerCount;
		data_s[StartGame_Field] = state->Field;
		std::copy(state->Characters.begin(),
				state->Characters.end(),
				&data_s[StartGame_Character]);
		NetworkManager::Broadcast(
				NetworkManager::StartGame, data_s);
		StateMachine::Change(
				new HostGameplayState(state->PlayerCount, state->Characters));
	}
}

void HostLobbyState::CloseLobby(void *)
{
	StateMachine::Change(new MainMenuState());
}
