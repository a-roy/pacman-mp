//! \file
//! SFData class declaration

#pragma once

#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

//! Contains data loaded by SFML
class SFData
{
	public:
		//! SFML RenderWindow
		static sf::RenderWindow *Window;
		//! SFML Texture database, where keys are file paths
		static std::map<std::string, sf::Texture> Textures;
		//! SFML Sprite database
		static std::vector<sf::Sprite> Sprites;

		//! Get a texture for the image at the given path, loading it into the
		//! database if it has not already been loaded.
		//!
		//! \path A path to an image file
		static const sf::Texture &GetTexture(std::string path);
};
