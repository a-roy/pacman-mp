//! \file
//! Renderer class declaration

#pragma once

#include <string>
#include "Sprite.h"

class Renderer
{
	public:
		static float Scale;

		static void Clear();
		static void Display();
		static void CreateWindow(int width, int height, std::string title);
		static int Renderer::CreateSprite(std::string texpath);
		static void DrawSprite(const Sprite &s, int x, int y, float theta, int anim, int frame);
		static void Deinit();
};
