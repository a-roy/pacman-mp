//! \file
//! StateMachine class declaration

#pragma once

#include "MainState.h"

class StateMachine
{
	public:
		static MainState *CurrentState;
		static void Change(MainState *nextState);
};
