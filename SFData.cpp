#include "SFData.h"

sf::RenderWindow *SFData::Window;
std::map<std::string, sf::Texture> SFData::Textures;
std::vector<sf::Sprite> SFData::Sprites;
std::map<std::string, sf::Font> SFData::Fonts;
std::vector<sf::Text> SFData::Texts;

const sf::Texture &SFData::GetTexture(std::string path)
{
	std::map<std::string, sf::Texture>::iterator i = Textures.find(path);
	if (i == Textures.end())
	{
		sf::Texture texture;
		texture.loadFromFile(path);
		Textures[path] = texture;
		return Textures[path];
	}
	else
	{
		return i->second;
	}
}

const sf::Font &SFData::GetFont(std::string path)
{
	std::map<std::string, sf::Font>::iterator i = Fonts.find(path);
	if (i == Fonts.end())
	{
		sf::Font font;
		font.loadFromFile(path);
		Fonts[path] = font;
		return Fonts[path];
	}
	else
	{
		return i->second;
	}
}
