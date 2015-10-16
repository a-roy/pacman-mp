//! \file
//! InputHandler class declaration

#pragma once

#include "Player.h"

class InputHandler
{
	public:
		static Player::Direction LastInput;
		static bool WindowClosed;

		static void PollEvents();
};
