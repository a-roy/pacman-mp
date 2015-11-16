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
		// TODO: confirm first byte
		// TODO: confirm identity of sender
		// TODO: overwrite `received` if invalid
	}
}

void NetworkManager::Send(MessageType mtype, char *data, std::size_t size,
		std::string address, unsigned short port)
{
	sf::IpAddress recipient = address;
	// TODO: insert correct header
	SFData::Socket.send(data, size, recipient, port);
}

void NetworkManager::Broadcast(MessageType mtype, char *data, std::size_t size,
		unsigned short port)
{
	sf::IpAddress recipient = sf::IpAddress::Broadcast;
	SFData::Socket.send(data, size, recipient, port);
}

std::string NetworkManager::GetAddress()
{
	// TODO: get public address
	return sf::IpAddress::getLocalAddress().toString();
}

unsigned short NetworkManager::GetPort()
{
	return SFData::Socket.getLocalPort();
}
