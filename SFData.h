//! \file
//! SFData class declaration

#pragma once

#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class SFData
{
	public:
		static sf::RenderWindow *Window;
		static std::map<std::string, sf::Texture> Textures;
		static std::vector<sf::Sprite> Sprites;

		static const sf::Texture &GetTexture(std::string path);
};
