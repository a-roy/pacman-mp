//! \file
//! NetworkManager class declaration

#pragma once

#include <cstddef>

class NetworkManager
{
	public:
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
};
