//! \file
//! NetworkManager class declaration

#pragma once

class NetworkManager
{
	public:
		void Init();
		void Receive(char *data, std::size_t &received);
};
