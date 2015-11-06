#include "NetworkManager.h"
#include "SFData.h"
#include <SFML/Network.hpp>

void NetworkManager::Init()
{
	SFData::Socket.setBlocking(false);
	SFData::Socket.bind(sf::Socket::AnyPort);
}

void NetworkManager::Receive(char *data, std::size_t &received)
{
	sf::IpAddress sender;
	unsigned short port;
	if (SFData::Socket.receive(data, 100, received, sender, port) ==
			sf::Socket::Done)
	{
		// confirm first byte
		// decode second byte
		// do something
	}
}
