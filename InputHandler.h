//! \file
//! InputHandler class declaration

#pragma once

#include "Direction.h"

class InputHandler
{
	public:
		static Direction LastInput;
		static int InputTime;
		static bool WindowClosed;

		static void PollEvents();
};
