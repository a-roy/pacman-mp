//! \file
//! Sprite class declaration

#pragma once

#include <vector>
#include "Animation.h"

class Sprite
{
	public:
		int Index;
		std::vector<Animation> Animations;
};
