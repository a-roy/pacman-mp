//! \file
//! MainState class implementation

#include "MainState.h"
#include "StateMachine.h"

void MainState::ProcessData()
{
	NetworkManager::LagIncrement();
	NetworkManager::MessageType mtype;
	std::vector<char> data_r;
	unsigned int id;
	NetworkManager::Receive(mtype, data_r, id);
	while (mtype != NetworkManager::None)
	{
		ProcessPacket(mtype, data_r, id);
		if (StateMachine::CurrentState != this)
		{
			return;
		}
		NetworkManager::Receive(mtype, data_r, id);
	}
}
