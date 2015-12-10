//! \file
//! MainMenuState class declaration

#include "MainState.h"

Menu MainMenuState::MenuItems;

void MainMenuState::LocalUpdate()
{
	MenuItems.Update();
}

void MainMenuState::Render() const
{
	MenuItems.Render(20, 100);
}
