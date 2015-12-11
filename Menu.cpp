#pragma once

#include "Menu.h"
#include "InputHandler.h"
#include "Renderer.h"

void Menu::AddItem(MenuItem *item)
{
	Items.push_back(item);
}

void Menu::Update()
{
	if (InputHandler::InputTime == 0)
	{
		if (InputHandler::LastInput == Down)
		{
			if (Index < Items.size() - 1)
			{
				Index++;
			}
		}
		else if (InputHandler::LastInput == Up)
		{
			if (Index > 0)
			{
				Index--;
			}
		}
		else if (InputHandler::LastInput == Right)
		{
			Items[Index]->Forward();
		}
		else if (InputHandler::LastInput == Left)
		{
			Items[Index]->Backward();
		}
	}
}

void Menu::Render(int x, int y) const
{
	for (std::size_t i = 0, size = Items.size(); i < size; i++)
	{
		Items[i]->Render(x + 48, y + 48 * i);
	}

	Renderer::DrawText(">", 24, x, y + 48 * Index);
}
