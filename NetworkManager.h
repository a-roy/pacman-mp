//! \file
//! NetworkManager class declaration

#pragma once

#include <cstddef>

class NetworkManager
{
	public:
		void Init();
		void Receive(char *data, std::size_t &received);
};
