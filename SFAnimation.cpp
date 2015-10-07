#include "SFAnimation.h"

SFAnimation::SFAnimation(int delay)
{
	Delay = delay;
}

sf::IntRect SFAnimation::getRect(int frame)
{
	return Frames[(frame / Delay) % Frames.size()];
}
