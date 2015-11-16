//! \file
//! NetworkManager class declaration

#pragma once

#include <cstddef>
#include <string>
#include <map>

class NetworkManager
{
	public:
		std::map<std::string, int> ConnectionAddresses;

		enum MessageType : unsigned char
		{
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

		void Init();
		void Receive(char *data, std::size_t &received);
		void Send(MessageType mtype, char *data, std::size_t size,
				std::string address, unsigned short port);
		void Broadcast(MessageType mtype, char *data, std::size_t size,
				unsigned short port);
		std::string GetAddress();
		unsigned short GetPort();
};
