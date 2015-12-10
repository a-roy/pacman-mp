//! \file
//! ClientConnectedState class implementation

#include "MainState.h"
#include "StateMachine.h"
#include "Renderer.h"
#include <sstream>

unsigned int ClientConnectedState::Index;
unsigned int ClientConnectedState::PlayerNumber;
Character ClientConnectedState::SelectedCharacter;
bool ClientConnectedState::Ready;
Game *ClientConnectedState::StartingGame;
Sprite ClientConnectedState::PacmanSprite;
std::vector<Sprite> ClientConnectedState::GhostSprites;
Menu ClientConnectedState::MenuItems;

void ClientConnectedState::Init()
{
	MenuItems.AddItem(new CharacterMenuItem(&SelectedCharacter));
	MenuItems.AddItem(new ReadyMenuItem(&Ready));

	PacmanSprite.Index = Renderer::CreateSprite("../spritesheet.png");
	Animation pac_move(4);
	pac_move.AddFrame(1, 1, 16, 16);
	pac_move.AddFrame(18, 1, 16, 16);
	pac_move.AddFrame(35, 1, 16, 16);
	pac_move.AddFrame(18, 1, 16, 16);
	PacmanSprite.Animations.push_back(pac_move);
	Animation pac_die(4);
	for (int i = 3; i <= 15; i++)
	{
		pac_die.AddFrame(1 + 17 * i, 1, 16, 16);
	}
	PacmanSprite.Animations.push_back(pac_die);

	GhostSprites.resize(4);
	for (int i = 0; i < 4; i++)
	{
		GhostSprites[i].Index =
			Renderer::CreateSprite("../spritesheet.png");
		Animation gho_hori(8);
		gho_hori.AddFrame(1, 18 + 17 * i, 16, 16);
		gho_hori.AddFrame(18, 18 + 17 * i, 16, 16);
		GhostSprites[i].Animations.push_back(gho_hori);
		Animation gho_up(8);
		gho_up.AddFrame(35, 18 + 17 * i, 16, 16);
		gho_up.AddFrame(52, 18 + 17 * i, 16, 16);
		GhostSprites[i].Animations.push_back(gho_up);
		Animation gho_down(8);
		gho_down.AddFrame(69, 18 + 17 * i, 16, 16);
		gho_down.AddFrame(86, 18 + 17 * i, 16, 16);
		GhostSprites[i].Animations.push_back(gho_down);
		Animation gho_fear(8);
		gho_fear.AddFrame(205, 18 + 17 * i, 16, 16);
		gho_fear.AddFrame(222, 18 + 17 * i, 16, 16);
		GhostSprites[i].Animations.push_back(gho_fear);
		Animation gho_flash(8);
		gho_flash.AddFrame(239, 18 + 17 * i, 16, 16);
		gho_flash.AddFrame(256, 18 + 17 * i, 16, 16);
		GhostSprites[i].Animations.push_back(gho_flash);
	}
}

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
			for (std::size_t i = 0; i < count; i++)
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
	Renderer::DrawText(str, 24, 60, 100);
	MenuItems.Render(20, 140);
}
