//! \file
//! Menu class declaration

#pragma once

#include "MenuItem.h"

class Menu
{
	public:
		void AddItem(MenuItem *item);
		void Update();
		void Render(int x, int y) const;
	private:
		std::vector<MenuItem *> Items;
		std::size_t Index = 0;
};
