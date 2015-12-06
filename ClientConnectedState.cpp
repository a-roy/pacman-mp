//! \file
//! ClientConnectedState class implementation

#include "MainState.h"
#include <sstream>

MainStateEnum ClientConnectedState::LocalUpdate()
{
	if (InputHandler::InputTime == 0)
	{
		if (Index == 0)
		{
			if (InputHandler::LastInput == Down)
			{
				Index = 1;
			}
			else if (InputHandler::LastInput == Left
					|| InputHandler::LastInput == Right)
			{
				if (SelectedCharacter == Pacman_c)
				{
					SelectedCharacter = Ghost_c;
				}
				else
				{
					SelectedCharacter = Pacman_c;
				}
			}
		}
		else
		{
			if (InputHandler::LastInput == Up)
			{
				Ready = false;
				Index = 0;
			}
			else if (InputHandler::LastInput == Right)
			{
				Ready = true;
				std::vector<char> data_s(PlayerReady_size);
				data_s[PlayerReady_Character] = SelectedCharacter;
				NetworkManager::Send(
						NetworkManager::PlayerReady, data_s, 0);
			}
			else if (InputHandler::LastInput == Left)
			{
				Ready = false;
				std::vector<char> data_s(PlayerNotReady_size);
				NetworkManager::Send(NetworkManager::PlayerNotReady, data_s, 0);
			}
		}
	}

	if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
	{
		return Join;
	}

	// ping server
	std::vector<char> data_s(PingServer_size);
	NetworkManager::Send(NetworkManager::PingServer, data_s, 0);

	return ClientConnected;
}

MainStateEnum ClientConnectedState::ProcessPacket(NetworkManager::MessageType mtype,
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
			PlayerNumber =
				data_r[ConfirmClient_PlayerNumber];
		}
		else if (mtype == NetworkManager::DisconnectClient)
		{
			return Join;
		}
		else if (mtype == NetworkManager::StartGame)
		{
			unsigned char count = data_r[StartGame_PlayerCount];
			unsigned char field = data_r[StartGame_Field];

			/// \TODO Add field selection
			Field gameField("../stage1.txt");
			Renderer::LoadField(&gameField, "../spritesheet.png");

			std::vector<Player *> players;
			for (unsigned int i = 0; i < count; i++)
			{
				Character c = (Character)data_r[StartGame_Character + i];
				if (c == Pacman_c)
				{
					players.push_back(new Pacman());
				}
				else if (c == Ghost_c)
				{
					players.push_back(new Ghost());
				}
			}
			StartingGame = new Game(gameField, players);
			return Gameplay;
		}
	}

	return ClientConnected;
}

void ClientConnectedState::Render() const
{
	std::ostringstream ss;
	ss << "You are Player ";
	ss << PlayerNumber + 1;
	std::string str = ss.str();
	Renderer::DrawText(0, str, 24, 60, 100);
	Character c = SelectedCharacter;
	bool ready = Ready;
	unsigned int index = Index;
	if (c == Pacman_c)
	{
		Renderer::DrawText(0, "< Pac-Man >", 18, 60, 140);
	}
	else
	{
		Renderer::DrawText(0, "<  Ghost  >", 18, 60, 140);
	}
	if (ready)
	{
		Renderer::DrawText(0, "< Ready!", 18, 60, 180);
	}
	else
	{
		Renderer::DrawText(0, "  Ready? >", 18, 60, 180);
	}
	Renderer::DrawText(0, ">", 18, 20, 140 + 40 * index);
}
