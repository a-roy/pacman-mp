//! \file
//! MainMenuState class declaration

#include "MainState.h"

MainStateEnum MainMenuState::LocalUpdate()
{
	if (InputHandler::InputTime == 0)
	{
		if (InputHandler::LastInput == Up)
		{
			if (Index > 0)
			{
				Index--;
			}
		}
		else if (InputHandler::LastInput == Down)
		{
			if (Index < MenuItems.size() - 1)
			{
				Index++;
			}
		}
		else if (InputHandler::LastInput == Right)
		{
			return MenuItems[Index].Function;
		}
	}

	return MainMenu;
}

void MainMenuState::Render() const
{
	for (unsigned int i = 0; i < MenuItems.size(); i++)
	{
		Renderer::DrawText(
				0, Menu_EN[MenuItems[i].Text], 24, 60, 100 + 40 * i);
		if (i == Index)
		{
			Renderer::DrawText(0, ">", 24, 20, 100 + 40 * i);
		}
	}
}
