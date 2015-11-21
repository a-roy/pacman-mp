#pragma once

#include "MainState.h"

MainState local_update(MainState state);
MainState local_main_menu(MainState state);
MainState local_join(MainState state);
MainState local_host(MainState state);
MainState local_client_waiting(MainState state);
MainState local_client_connected(MainState state);
MainState local_gameplay(MainState state);
void addrIncrement(unsigned int digit, int amount);

