//! \file
//! ClientWaitingState class implementation

#include "MainState.h"

MainStateEnum ClientWaitingState::LocalUpdate()
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

	return ClientWaiting;
}

MainStateEnum ClientWaitingState::ProcessPacket(
		NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	if (id == 0)
	{
		if (mtype == NetworkManager::ConfirmClient)
		{
			PlayerNumber = data_r[ConfirmClient_PlayerNumber];
			return ClientConnected;
		}
	}

	return ClientWaiting;
}

void ClientWaitingState::Render() const
{
	Renderer::DrawText(0, "Connecting...", 24, 60, 100);
}
