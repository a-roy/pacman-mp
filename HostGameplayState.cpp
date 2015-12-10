//! \file
//! HostGameplayState class implementation

#include "MainState.h"
#include <sstream>

MainStateEnum HostGameplayState::ProcessPacket(
		NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	for (unsigned int i = 0; i < PlayerCount; i++)
	{
		if (NetworkManager::CurrentConnections[i].Lag > NetworkTimeout)
		{
			std::vector<char> data_s(EndGame_size);
			NetworkManager::Broadcast(NetworkManager::EndGame, data_s);
			return MainMenu;
		}
	}

	if (mtype == NetworkManager::PingServer)
	{
		std::vector<char> data_s(StartGame_minsize + PlayerCount);
		data_s[StartGame_PlayerCount] = (char)PlayerCount;
		data_s[StartGame_Field] = 0;
		std::copy(Characters.begin(),
				Characters.end(),
				&data_s[StartGame_Character]);
		NetworkManager::Send(NetworkManager::StartGame, data_s, id);
	}
	else if (mtype == NetworkManager::OwnInputs)
	{
		std::vector<char> data_s(OtherInputs_size);
		data_s[OtherInputs_PlayerNumber] = id;
		std::copy(
				&data_r[OwnInputs_Frame],
				&data_r[OwnInputs_Frame] + Frame_size,
				&data_s[OtherInputs_Frame]);
		std::copy(
				&data_r[OwnInputs_InputData],
				&data_r[OwnInputs_InputData] + InputData_size,
				&data_s[OtherInputs_InputData]);
		NetworkManager::Broadcast(NetworkManager::OtherInputs, data_s);
	}
	else if (mtype == NetworkManager::EndedGame)
	{
		GameEnded[id] = true;
		for (std::size_t i = 0; i < GameEnded.size(); i++)
		{
			if (!GameEnded[i])
			{
				return HostGameplay;
			}
		}
		std::vector<char> data_s(EndGame_size);
		NetworkManager::Broadcast(NetworkManager::EndGame, data_s);
		return MainMenu;
	}

	return HostGameplay;
}

void HostGameplayState::Render() const
{
	std::ostringstream ss;
	ss << "Clients connected: " << PlayerCount;
	std::string str = ss.str();
	Renderer::DrawText(0, str, 24, 60, 100);
}
