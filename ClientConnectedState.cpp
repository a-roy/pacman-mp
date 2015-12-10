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
			else if (InputHandler::LastInput == Left)
			{
				if (SelectedCharacter == PacMan)
				{
					SelectedCharacter = Clyde;
				}
				else
				{
					SelectedCharacter = (Character)(SelectedCharacter - 1);
				}
			}
			else if (InputHandler::LastInput == Right)
			{
				if (SelectedCharacter == Clyde)
				{
					SelectedCharacter = PacMan;
				}
				else
				{
					SelectedCharacter = (Character)(SelectedCharacter + 1);
				}
			}
		}
		else
		{
			if (InputHandler::LastInput == Up)
			{
				Ready = false;
				std::vector<char> data_s(PlayerNotReady_size);
				NetworkManager::Send(NetworkManager::PlayerNotReady, data_s, 0);
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

			Field gameField;
			if (field == 0U)
			{
				gameField = Field("../stage1.txt");
			}
			else
			{
				gameField = Field("../stage2.txt");
			}
			Renderer::LoadField(&gameField, "../spritesheet.png");

			std::vector<Player *> players;
			for (unsigned int i = 0; i < count; i++)
			{
				Character c = (Character)data_r[StartGame_Character + i];
				if (c == PacMan)
				{
					players.push_back(new Pacman(PacmanSprite,
								Position(
									14 * TILE_SIZE,
									23 * TILE_SIZE + (TILE_SIZE - 1) / 2),
								Left));
				}
				else if (c == Blinky)
				{
					players.push_back(new Ghost(GhostSprites[0],
								Position(
									14 * TILE_SIZE,
									11 * TILE_SIZE + (TILE_SIZE - 1) / 2),
								Left));
				}
				else if (c == Inky)
				{
					players.push_back(new Ghost(GhostSprites[2],
								Position(
									12 * TILE_SIZE,
									14 * TILE_SIZE + (TILE_SIZE - 1) / 2),
								Up));
				}
				else if (c == Pinky)
				{
					players.push_back(new Ghost(GhostSprites[1],
								Position(
									14 * TILE_SIZE,
									14 * TILE_SIZE + (TILE_SIZE - 1) / 2),
								Down));
				}
				else if (c == Clyde)
				{
					players.push_back(new Ghost(GhostSprites[3],
								Position(
									16 * TILE_SIZE,
									14 * TILE_SIZE + (TILE_SIZE - 1) / 2),
								Up));
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

	if (c == PacMan)
	{
		Renderer::DrawText(0, "< Pac-Man    >", 18, 60, 140);
		Renderer::DrawSprite(
				PacmanSprite,
				9 * TILE_SIZE, 5 * TILE_SIZE,
				0.f, true,
				0, 4);
	}
	else if (c == Blinky)
	{
		Renderer::DrawText(0, "< Blinky     >", 18, 60, 140);
		Renderer::DrawSprite(
				GhostSprites[0],
				9 * TILE_SIZE, 5 * TILE_SIZE,
				0.f, false,
				0, 0);
	}
	else if (c == Inky)
	{
		Renderer::DrawText(0, "< Inky       >", 18, 60, 140);
		Renderer::DrawSprite(
				GhostSprites[2],
				9 * TILE_SIZE, 5 * TILE_SIZE,
				0.f, false,
				0, 0);
	}
	else if (c == Pinky)
	{
		Renderer::DrawText(0, "< Pinky      >", 18, 60, 140);
		Renderer::DrawSprite(
				GhostSprites[1],
				9 * TILE_SIZE, 5 * TILE_SIZE,
				0.f, false,
				0, 0);
	}
	else if (c == Clyde)
	{
		Renderer::DrawText(0, "< Clyde      >", 18, 60, 140);
		Renderer::DrawSprite(
				GhostSprites[3],
				9 * TILE_SIZE, 5 * TILE_SIZE,
				0.f, false,
				0, 0);
	}

	if (Ready)
	{
		Renderer::DrawText(0, "< Ready!", 18, 60, 180);
	}
	else
	{
		Renderer::DrawText(0, "  Ready? >", 18, 60, 180);
	}
	Renderer::DrawText(0, ">", 18, 20, 140 + 40 * Index);
}
