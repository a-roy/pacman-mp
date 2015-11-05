#include "NetworkManager.h"
#include <SFML/Network.hpp>

void NetworkManager::Init()
{
	SFData::Socket.setBlocking(false);
	SFData::Socket.bind(sf::Socket::AnyPort);
}

void NetworkManager::Receive(char *data, std::size_t &received)
{
	if (SFData::Socket.receive(data, 100, received) == sf::Socket::Done)
	{
		// do something
	}
}
