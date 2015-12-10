//! \file
//! ClientConnectedState class implementation

#include "MainState.h"
#include "StateMachine.h"
#include <sstream>

unsigned int ClientConnectedState::Index;
unsigned int ClientConnectedState::PlayerNumber;
Character ClientConnectedState::SelectedCharacter;
bool ClientConnectedState::Ready;
Game *ClientConnectedState::StartingGame;
Sprite ClientConnectedState::PacmanSprite;
std::vector<Sprite> ClientConnectedState::GhostSprites;
Menu ClientConnectedState::MenuItems;

void ClientConnectedState::Change()
{
	PlayerNumber = ClientWaitingState::PlayerNumber;
	SelectedCharacter = PacMan;
	Ready = false;
	StartingGame = NULL;
}

void ClientConnectedState::LocalUpdate()
{
	MenuItems.Update();

	if (NetworkManager::CurrentConnections[0].Lag > NetworkTimeout)
	{
		StateMachine::Change(new JoinState());
		return;
	}

	// ping server
	std::vector<char> data_s(PingServer_size);
	NetworkManager::Send(NetworkManager::PingServer, data_s, 0);
}

void ClientConnectedState::ProcessPacket(NetworkManager::MessageType mtype,
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
			StateMachine::Change(new JoinState());
			return;
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
			StateMachine::Change(new GameplayState());
		}
	}
}

void ClientConnectedState::Render() const
{
	std::ostringstream ss;
	ss << "You are Player ";
	ss << PlayerNumber + 1;
	std::string str = ss.str();
	Renderer::DrawText(0, str, 24, 60, 100);
	MenuItems.Render(20, 140);
}
