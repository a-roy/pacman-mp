//! \file
//! NetworkManager class declaration

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#define NetworkTimeout 120
#define NetworkDelay 2

#define InputData_size 10
#define Frame_size 2
#define ConfirmClient_size 1
#define ConfirmClient_PlayerNumber 0
#define PingClient_size 0
#define StartGame_minsize 2
#define StartGame_PlayerCount 0
#define StartGame_Field 1
#define StartGame_Character 2
#define OtherInputs_size (InputData_size + Frame_size + 1)
#define OtherInputs_PlayerNumber 0
#define OtherInputs_Frame 1
#define OtherInputs_InputData (Frame_size + 1)
#define BlockFrame_size 0
#define EndGame_size 0
#define DisconnectClient_size 0
#define RequestServer_size 0
#define PingServer_size 0
#define PlayerReady_size 1
#define PlayerReady_Character 0
#define PlayerNotReady_size 0
#define OwnInputs_size (InputData_size + Frame_size)
#define OwnInputs_Frame 0
#define OwnInputs_InputData (Frame_size)
#define EndedGame_size 0
#define DisconnectServer_size 0

class NetworkManager
{
	public:
		struct Connection
		{
			std::string Address;
			unsigned int AddressIndex;
			unsigned short Port;
			unsigned int Lag;
		};
		static std::vector<Connection> CurrentConnections;

		enum MessageType : unsigned char
		{
			None,
			ConfirmClient,
			PingClient,
			StartGame,
			OtherInputs,
			BlockFrame,
			EndGame,
			DisconnectClient,
			RequestServer,
			PingServer,
			PlayerReady,
			PlayerNotReady,
			OwnInputs,
			EndedGame,
			DisconnectServer
		};

		static void Init();
		static void Receive(MessageType &mtype, std::vector<char>& data,
				unsigned int &sender);
		static void Send(MessageType mtype, const std::vector<char> &data,
				unsigned int recipient);
		static void Broadcast(MessageType mtype, const std::vector<char> &data);
		static bool HasConnection(std::string address, unsigned short port);
		static unsigned int
			GetConnection(std::string address, unsigned short port);
		static void LagIncrement();
		static void ResetConnections();
		static std::string GetAddress();
		static unsigned short GetPort();
};
