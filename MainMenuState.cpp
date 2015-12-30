//! \file
//! MainMenuState class declaration

#include "MainState.h"
#include "StateMachine.h"

Menu MainMenuState::MenuItems;

void MainMenuState::Change()
{
	NetworkManager::ResetConnections();
}

void MainMenuState::Init()
{
	MenuItems.AddItem(
			new FunctionalMenuItem<void>("Host game",
			[](void *){ StateMachine::Change(new HostLobbyState()); },
			[](void *){ }));
	MenuItems.AddItem(
			new FunctionalMenuItem<void>("Join game",
			[](void *){ StateMachine::Change(new JoinState()); },
			[](void *){ }));
	MenuItems.AddItem(
			new FunctionalMenuItem<void>("Quit",
			[](void *){ StateMachine::Change(new ExitingState()); },
			[](void *){ }));
}

void MainMenuState::LocalUpdate()
{
	MenuItems.Update();
}

void MainMenuState::Render() const
{
	MenuItems.Render(20, 100);
}
