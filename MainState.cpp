//! \file
//! MainState class implementation

#include "MainState.h"

MainStateEnum MainState::ProcessData()
{
	NetworkManager::LagIncrement();
	NetworkManager::MessageType mtype;
	std::vector<char> data_r;
	unsigned int id;
	NetworkManager::Receive(mtype, data_r, id);
	while (mtype != NetworkManager::None)
	{
		MainStateEnum s = ProcessPacket(mtype, data_r, id);
		if (s != GetEnum())
		{
			return s;
		}
		NetworkManager::Receive(mtype, data_r, id);
	}

	return GetEnum();
}
