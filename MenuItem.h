//! \file
//! MenuItem struct declaration

#pragma once

#include <vector>
#include "MainState.h"

struct MenuItem
{
	int Text;
	MainState Function;
};

typedef std::vector<MenuItem> Menu;
