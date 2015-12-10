#include "StateMachine.h"

MainState *StateMachine::CurrentState;

void StateMachine::Change(MainState *nextState)
{
	delete CurrentState;
	CurrentState = nextState;
	nextState->Change();
}
