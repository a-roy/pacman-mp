//! \file
//! Sprite class declaration

#pragma once

#include <vector>
#include "Animation.h"

//! Represents a drawable animated sprite
class Sprite
{
	public:
		//! Holds an index referring to a library-specific database with
		//! rendering information for this sprite (e.g. SFData::Sprites)
		int Index;
		//! Contains animation information for this sprite
		std::vector<Animation> Animations;
};
