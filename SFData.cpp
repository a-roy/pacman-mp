#include "SFData.h"

sf::RenderWindow *SFData::Window;
std::map<std::string, sf::Texture> SFData::Textures;
std::vector<sf::Sprite> SFData::Sprites;

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
