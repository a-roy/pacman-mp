#pragma once

#include <vector>
#include "MainState.h"
#include "NetworkManager.h"

MainState process_data(MainState state);
MainState process_packet(MainState state, NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id);
MainState process_host(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id);
MainState process_client_waiting(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id);
MainState process_client_connected(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id);
MainState process_gameplay(NetworkManager::MessageType mtype,
		std::vector<char> &data_r, unsigned int id);
