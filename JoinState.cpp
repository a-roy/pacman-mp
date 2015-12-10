//! \file
//! JoinState class declaration

#include "MainState.h"
#include "StateMachine.h"
#include "NetworkManager.h"
#include <iomanip>
#include <sstream>

unsigned int JoinState::Index;
unsigned char JoinState::IP[4];
unsigned short JoinState::Port;

void JoinState::Init()
{
	Index = 0;
	std::string myAddress = NetworkManager::GetAddress();
	std::stringstream addr(myAddress);
	std::string part;
	for (std::size_t i = 0; i < 4 && std::getline(addr, part, '.'); i++)
	{
		std::stringstream num(part);
		unsigned short n;
		num >> n;
		IP[i] = n;
	}
	Port = 0;
}

void JoinState::LocalUpdate()
{
	if (InputHandler::InputTime == 0)
	{
		if (InputHandler::LastInput == Left)
		{
			if (Index > 0)
			{
				Index--;
			}
			else
			{
				StateMachine::Change(new MainMenuState());
			}
		}
		else if (InputHandler::LastInput == Right)
		{
			if (Index < 16)
			{
				Index++;
			}
			else
			{
				std::ostringstream ss;
				ss << (unsigned short)IP[0] << "."
					<< (unsigned short)IP[1] << "."
					<< (unsigned short)IP[2] << "."
					<< (unsigned short)IP[3];
				NetworkManager::ResetConnections();
				NetworkManager::GetConnection(ss.str(), Port);
				StateMachine::Change(new ClientWaitingState());
			}
		}
		else if (InputHandler::LastInput == Up)
		{
			AddrIncrement(1);
		}
		else if (InputHandler::LastInput == Down)
		{
			AddrIncrement(-1);
		}
	}
}

void JoinState::Render() const
{
	std::ostringstream ss;
	ss.fill('0');
	ss << std::setw(3) << (unsigned short)IP[0] << "."
		<< std::setw(3) << (unsigned short)IP[1] << "."
		<< std::setw(3) << (unsigned short)IP[2] << "."
		<< std::setw(3) << (unsigned short)IP[3] << ":"
		<< std::setw(5) << Port;
	std::string disp = ss.str();
	Renderer::DrawText(disp, 24, 60, 100);
	int iPos = Index;
	if (Index > 2) iPos++;
	if (Index > 5) iPos++;
	if (Index > 8) iPos++;
	if (Index > 11) iPos++;
	Renderer::DrawText("^", 24, 60 + 24 * iPos, 136);
}

void JoinState::AddrIncrement(int amount)
{
	int delta = amount;
	if (Index < 12)
	{
		for (unsigned int i = Index % 3; i < 2; i++)
		{
			delta *= 10;
		}
		unsigned char &selected = IP[Index / 3];
		if (selected + delta == (selected + delta + 255) % 255)
		{
			selected += delta;
		}
		else
		{
			selected = (amount > 0) ? 255 : 0;
		}
	}
	else
	{
		for (unsigned int i = Index - 12; i < 4; i++)
		{
			delta *= 10;
		}
		unsigned short &port = Port;
		if (port + delta == (port + delta + 65535) % 65535)
		{
			port += delta;
		}
		else
		{
			port = (amount > 0) ? 65535 : 0;
		}
	}
}
