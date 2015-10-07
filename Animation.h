//! \file
//! Animation class declaration

#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
	std::vector<sf::IntRect> Frames;
	int Delay;
};
