//! \file
//! MainMenuState class declaration

#include "MainState.h"
#include "StateMachine.h"

Menu MainMenuState::MenuItems;

void MainMenuState::Init()
{
	MenuItems.AddItem(
			new FunctionalMenuItem("Host game",
			[](){ StateMachine::Change(new HostLobbyState()); },
			[](){ }));
	MenuItems.AddItem(
			new FunctionalMenuItem("Join game",
			[](){ StateMachine::Change(new JoinState()); },
			[](){ }));
	MenuItems.AddItem(
			new FunctionalMenuItem("Quit",
			[](){ StateMachine::Change(new ExitingState()); },
			[](){ }));
}

void MainMenuState::LocalUpdate()
{
	MenuItems.Update();
}

void MainMenuState::Render() const
{
	MenuItems.Render(20, 100);
}
