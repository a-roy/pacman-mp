//! \file
//! SFData class declaration

#pragma once

#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

//! Contains data loaded by SFML
class SFData
{
	public:
		//! SFML RenderWindow
		static sf::RenderWindow *Window;
		//!
		static sf::UdpSocket Socket;
		//! SFML Texture database, where keys are file paths
		static std::map<std::string, sf::Texture> Textures;
		//! SFML Sprite database
		static std::vector<sf::Sprite> Sprites;
		//! SFML Font database, where keys are file paths
		static std::map<std::string, sf::Font> Fonts;
		//! SFML IP database
		static std::vector<sf::IpAddress> Addresses;

		//! Get a texture for the image at the given path, loading it into the
		//! database if it has not already been loaded.
		//!
		//! \path A path to an image file
		static const sf::Texture &GetTexture(std::string path);
		//! Get a font for the font file at the given path, loading it into the
		//! database if it has not already been loaded
		//!
		//! \path A path to a font file
		static const sf::Font &GetFont(std::string path);
};
