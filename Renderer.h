//! \file
//! Renderer class declaration

#pragma once

#include <cstdint>
#include <string>
#include "Sprite.h"
#include "Field.h"

//! Handles graphics rendering
class Renderer
{
	public:
		//! A scaling factor for all drawing coordinates
		static float TileScale;
		static float SpriteScale;

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
		//!                SFData::Sprites)
		static int CreateSprite(std::string texpath);
		static void LoadField(Field *field);
		//! Load a font
		//!
		//! \param fontpath The path to the corresponding font file
		//! \returns        The index of the font in the database (e.g.
		//!                 SFData::Fonts)
		static int LoadFont(std::string fontpath);
		//! Draw a sprite to the window
		//!
		//! \param s     The sprite to draw
		//! \param x     The X-coordinate at which to draw the sprite
		//! \param y     The Y-coordinate at which to draw the sprite
		//! \param theta The rotation (in degrees) of the sprite
		//! \param anim  The index of the sprite's current animation
		//! \param frame The sprite's current frame of animation
		static void DrawSprite(const Sprite &s, int x, int y, float theta,
				bool flip, int anim, int frame);
		static void DrawField(std::array<uint32_t, FIELD_HEIGHT> eaten);
		//! Draw a line of text to the window
		//!
		//! \param fontIndex The database index of a font
		//! \param text      The string of text to display
		//! \param charSize  The character size to use for drawing
		//! \param x         The X-coordinate at which to draw the text
		//! \param y         The Y-coordinate at which to draw the text
		static void DrawText(int fontIndex, std::string text,
				unsigned int charSize, int x, int y);
		//! Deinitialize the graphics window
		static void Deinit();
};
