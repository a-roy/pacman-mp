//! \file
//! InputHandler class declaration

#pragma once

#include "Position.h"

class InputHandler
{
	public:
		static Position LastInput;
		static int InputTime;
		static bool WindowClosed;

		static void PollEvents();
};
