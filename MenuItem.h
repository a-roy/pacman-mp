//! \file
//! MenuItem struct declaration

#pragma once

#include <vector>
#include "MainStateEnum.h"

struct MenuItem
{
	int Text;
	MainStateEnum Function;
};

typedef std::vector<MenuItem> Menu;
