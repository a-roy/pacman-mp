//! \file
//! NetworkManager class declaration

#pragma once

#include <cstddef>
#include <string>
#include <vector>

class NetworkManager
{
	public:
		struct Connection
		{
			std::string Address;
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
