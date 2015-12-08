//! \file
//! GameplayState class implementation

#include "MainState.h"
#include <algorithm>

MainStateEnum GameplayState::LocalUpdate()
{
	// Instead of changing our frame number, we use the delay to read inputs
	// from previous frames
	if ((Local->CurrentFrame - Synced->CurrentFrame + 1 + NetworkDelay) * 2
			< InputData_size)
	{
		PlayerInputs[PlayerNumber].erase(PlayerInputs[PlayerNumber].begin());
		PlayerInputs[PlayerNumber].push_back(InputHandler::LastInput);
		for (unsigned int i = 0; i < PlayerInputs.size(); i++)
		{
			Local->Players[i]->SetDirection(
				PlayerInputs[i][InputData_size - 1 - NetworkDelay]);
		}
		Local->update();
	}

	unsigned int currentFrame = Local->CurrentFrame;
	int f = currentFrame + NetworkDelay - 1;
	ReceivedFrames[PlayerNumber] = f;

	unsigned short all_received = *std::min_element(
			std::begin(ReceivedFrames), std::end(ReceivedFrames));
	while (Synced->CurrentFrame < all_received
			&& Synced->CurrentFrame < currentFrame)
	{
		for (unsigned int i = 0; i < ReceivedFrames.size(); i++)
		{
			Synced->Players[i]->SetDirection(PlayerInputs[i][
				InputData_size - NetworkDelay
					+ Synced->CurrentFrame - Local->CurrentFrame]);
		}
		bool ongoing;
		ongoing = Synced->update();
		if (!ongoing)
		{
			std::vector<char> data_s(EndedGame_size);
			NetworkManager::Send(NetworkManager::EndedGame, data_s, 0);
			return MainMenu;
		}
	}

	*Local = *Synced;
	while (Local->CurrentFrame < currentFrame)
	{
		Local->Players[PlayerNumber]->SetDirection(PlayerInputs[PlayerNumber][
			InputData_size - NetworkDelay
				+ Local->CurrentFrame - currentFrame]);
		Local->update();
	}

	std::vector<char> data_s(OwnInputs_size);
	for (int i = OwnInputs_Frame + Frame_size - 1;
			i >= OwnInputs_Frame; i--)
	{
		data_s[i] = (char)(f & 0xFF);
		f = f >> 8;
	}
	std::transform(
			PlayerInputs[PlayerNumber].begin(),
			PlayerInputs[PlayerNumber].end(),
			&data_s[OwnInputs_InputData],
			Position::ByteEncode);
	NetworkManager::Send(NetworkManager::OwnInputs, data_s, 0);

	return Gameplay;
}

MainStateEnum GameplayState::ProcessPacket(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id)
{
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
			int difference = Local->CurrentFrame + NetworkDelay - f;
			if (num != PlayerNumber && f > ReceivedFrames[num])
			{
				if (difference >= 0)
				{
					std::transform(
							&data_r[OtherInputs_InputData] + difference,
							&data_r[OtherInputs_InputData] + InputData_size,
							&PlayerInputs[num][0],
							Position::ByteDecode);
					Position d = Position::ByteDecode(
						data_r[OtherInputs_InputData + InputData_size - 1]);
					if (difference > 0)
					{
						std::fill(
								&PlayerInputs[num][InputData_size - difference],
								&PlayerInputs[num][0] + InputData_size, d);
					}
					ReceivedFrames[num] = f;
				}
				else
				{
					std::transform(
						&data_r[OtherInputs_InputData],
						&data_r[OtherInputs_InputData]
						+ InputData_size + difference,
						&PlayerInputs[num][0] - difference,
						Position::ByteDecode);
					ReceivedFrames[num] = Local->CurrentFrame + NetworkDelay;
				}
			}
		}
	}

	return Gameplay;
}

void GameplayState::Render() const
{
	Renderer::DrawField(Local->Pellets);
	Local->draw();
}
