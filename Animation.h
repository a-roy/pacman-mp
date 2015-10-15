//! \file
//! Animation class declaration

#pragma once

#include <vector>

class Animation
{
	public:
		struct Rectangle
		{
			int X, Y, Width, Height;
		};

		std::vector<Rectangle> Frames;
		int Delay;

		Animation(int delay);
		void AddFrame(int x, int y, int width, int height);
		void GetRect(int frame, int &x, int &y, int &width, int &height) const;
};
