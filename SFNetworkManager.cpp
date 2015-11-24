#include "NetworkManager.h"
#include "SFData.h"
#include <SFML/Network.hpp>

std::vector<NetworkManager::Connection> NetworkManager::CurrentConnections;

void NetworkManager::Init()
{
	SFData::Socket.setBlocking(false);
	SFData::Socket.bind(sf::Socket::AnyPort);
}

void NetworkManager::Receive(MessageType &mtype, std::vector<char> &data,
		unsigned int &sender)
{
	char d[100];
	std::size_t received;
	sf::IpAddress senderIp;
	unsigned short port;
	if (SFData::Socket.receive(d, 100, received, senderIp, port) ==
			sf::Socket::Done)
	{
		// TODO: confirm first byte
		// TODO: confirm identity of sender & reset timeout
		if (received > 1)
		{
			mtype = (MessageType)d[1];
			data = std::vector<char>(received - 2);
			std::copy(d + 2, d + received, data.begin());
		}
		else
		{
			mtype = None;
		}
		sender = GetConnection(senderIp.toString(), port);
	}
	else
	{
		mtype = None;
	}
}

void NetworkManager::Send(MessageType mtype, const std::vector<char> &data,
		unsigned int recipient)
{
	Connection c = CurrentConnections[recipient];
	sf::IpAddress &address = SFData::Addresses[c.AddressIndex];
	unsigned short port = c.Port;
	std::size_t size = 2 + data.size();
	char *d = (char *)malloc(size * sizeof(char));
	// TODO: insert correct header
	d[0] = 0x00;
	d[1] = mtype;
	std::copy(data.begin(), data.end(), d + 2);
	SFData::Socket.send(d, size, address, port);
	free(d);
}

void NetworkManager::Broadcast(MessageType mtype, const std::vector<char> &data)
{
	for (unsigned int i = 0, size = CurrentConnections.size(); i < size; i++)
	{
		Send(mtype, data, i);
	}
}

bool NetworkManager::HasConnection(std::string address, unsigned short port)
{
	for (unsigned int i = 0, size = CurrentConnections.size(); i < size; i++)
	{
		if (CurrentConnections[i].Address == address
				&& CurrentConnections[i].Port == port)
		{
			return true;
		}
	}
	return false;
}

unsigned int
NetworkManager::GetConnection(std::string address, unsigned short port)
{
	for (unsigned int i = 0, size = CurrentConnections.size(); i < size; i++)
	{
		if (CurrentConnections[i].Address == address
				&& CurrentConnections[i].Port == port)
		{
			return i;
		}
	}
	Connection c;
	c.AddressIndex = SFData::Addresses.size();
	sf::IpAddress ipAddress = address;
	SFData::Addresses.push_back(ipAddress);
	c.Address = ipAddress.toString();
	c.Port = port;
	c.Lag = 0;
	CurrentConnections.push_back(c);
	return CurrentConnections.size() - 1;
}

void NetworkManager::LagIncrement()
{
	for (unsigned int i = 0, end = CurrentConnections.size(); i < end; i++)
	{
		CurrentConnections[i].Lag++;
	}
}

void NetworkManager::ResetConnections()
{
	CurrentConnections.clear();
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
