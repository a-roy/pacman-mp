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
		//! SFML RenderTexture for backgrounds
		static sf::RenderTexture *FieldTexture;
		//!
		static sf::Shader FieldShader;
		//!
		static sf::Texture PelletTexture;
		//!
		static sf::UdpSocket Socket;
		//! SFML Texture database, where keys are file paths
		static std::map<std::string, sf::Texture> Textures;
		//! SFML Sprite database
		static std::vector<sf::Sprite> Sprites;
		//! SFML Font database
		static std::vector<sf::Font> Fonts;
		//! SFML IP database
		static std::vector<sf::IpAddress> Addresses;

		//! Get a texture for the image at the given path, loading it into the
		//! database if it has not already been loaded.
		//!
		//! \path A path to an image file
		static const sf::Texture &GetTexture(std::string path);
};
