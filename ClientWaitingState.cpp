//! \file
//! ClientWaitingState class implementation

#include "MainState.h"
#include "StateMachine.h"

unsigned int ClientWaitingState::PlayerNumber;

void ClientWaitingState::LocalUpdate()
{
	if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
	{
		StateMachine::Change(new JoinState());
	}
	else
	{
		std::vector<char> data_s(RequestServer_size);
		NetworkManager::Send(NetworkManager::RequestServer, data_s, 0);
	}
}

void ClientWaitingState::ProcessPacket(
		NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
	if (id == 0)
	{
		if (mtype == NetworkManager::ConfirmClient)
		{
			PlayerNumber = data_r[ConfirmClient_PlayerNumber];
			StateMachine::Change(new ClientConnectedState());
		}
	}
}

void ClientWaitingState::Render() const
{
	Renderer::DrawText(0, "Connecting...", 24, 60, 100);
}
