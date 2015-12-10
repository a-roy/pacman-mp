//! \file
//! Animation class declaration

#pragma once

#include <vector>

//! Contains animation data used by the Sprite class
class Animation
{
	public:
		//! A simple data structure for describing texture coordinates
		struct Rectangle
		{
			int X, Y, Width, Height;
		};

		//! Constructor
		//!
		//! \param delay Delay between animation frames
		Animation(int delay);
		//! Add a frame to the end of this animation with the given texture coordinates
		//!
		//! \param x      X-coordinate
		//! \param y      Y-coordinate
		//! \param width  Width
		//! \param height Height
		void AddFrame(int x, int y, int width, int height);
		//! Get the texture coordinates for rendering an animation frame
		//!
		//! \param[in] frame   Current animation frame
		//! \param[out] x      X-coordinate
		//! \param[out] y      Y-coordinate
		//! \param[out] width  Width
		//! \param[out] height Height
		void GetRect(int frame, int &x, int &y, int &width, int &height) const;

	private:
		//! Texture coordinates for each animation frame
		std::vector<Rectangle> Frames;
		//! Length of each animation frame
		int Delay;
};
