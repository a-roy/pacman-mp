#include "SFData.h"

sf::RenderWindow *SFData::Window;
sf::RenderTexture *SFData::FieldTexture;
sf::Shader SFData::FieldShader;
sf::Texture SFData::PelletTexture;
sf::UdpSocket SFData::Socket;
std::map<std::string, sf::Texture> SFData::Textures;
std::vector<sf::Sprite> SFData::Sprites;
sf::Font SFData::Font;
std::vector<sf::IpAddress> SFData::Addresses;

const sf::Texture &SFData::GetTexture(std::string path)
{
	std::map<std::string, sf::Texture>::iterator i = Textures.find(path);
	if (i == Textures.end())
	{
		sf::Image image;
		image.loadFromFile(path);
		image.createMaskFromColor(sf::Color(0, 0, 0));
		sf::Texture texture;
		texture.loadFromImage(image);
		Textures[path] = texture;
		return Textures[path];
	}
	else
	{
		return i->second;
	}
}
