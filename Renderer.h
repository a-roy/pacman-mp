//! \file
//! Renderer class declaration

#pragma once

#include <string>
#include "Sprite.h"

//! Handles graphics rendering
class Renderer
{
	public:
		//! A scaling factor for all drawing coordinates
		static float Scale;

		//! Clear the screen
		static void Clear();
		//! Display next frame
		static void Display();
		//! Create a window
		static void CreateWindow(int width, int height, std::string title);
		//! Check if the window is still open
		static bool WindowOpen();
		//! Create a sprite
		//!
		//! \param texpath The path to the corresponding texture file
		//! \returns       The index of the sprite in the database (e.g.
		//! SFData::Sprites)
		static int CreateSprite(std::string texpath);
		//! Draw a sprite to the window
		//!
		//! \param s     The sprite to draw
		//! \param x     The X-coordinate at which to draw the sprite
		//! \param y     The Y-coordinate at which to draw the sprite
		//! \param theta The rotation (in degrees) of the sprite
		//! \param anim  The index of the sprite's current animation
		//! \param frame The sprite's current frame of animation
		static void DrawSprite(
				const Sprite &s, int x, int y, float theta,
				int anim, int frame);
		//! Deinitialize the graphics window
		static void Deinit();
};
