//! \file
//! SFAnimation class declaration

#include <vector>
#include <SFML/Graphics.hpp>

class SFAnimation
{
public:
	std::vector<sf::IntRect> Frames;
	int Delay;

	SFAnimation(int delay);
	sf::IntRect getRect(int frame);
};
