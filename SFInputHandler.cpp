#include "InputHandler.h"
#include "SFML/Window.hpp"
#include "SFData.h"

Player::Direction InputHandler::LastInput;
int InputHandler::InputTime;
bool InputHandler::WindowClosed = false;

void InputHandler::PollEvents()
{
    sf::Event event;
    while (SFData::Window->pollEvent(event))
    {
		Player::Direction newInput = (Player::Direction)-1;
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::W:
						newInput = Player::Up;
						break;
					case sf::Keyboard::A:
						newInput = Player::Left;
						break;
					case sf::Keyboard::S:
						newInput = Player::Down;
						break;
					case sf::Keyboard::D:
						newInput = Player::Right;
						break;
					default:
						continue;
				}
				LastInput = newInput;
				InputTime = -1;
				break;
			case sf::Event::Closed:
				WindowClosed = true;
				break;
		}
	}
	InputTime++;
}
