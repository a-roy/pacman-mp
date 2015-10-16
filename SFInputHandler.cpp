#include "InputHandler.h"
#include "SFML/Window.hpp"
#include "SFData.h"

Player::Direction InputHandler::LastInput;
bool InputHandler::WindowClosed = false;

void InputHandler::PollEvents()
{
    sf::Event event;
    while (SFData::Window->pollEvent(event))
    {
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::W:
						LastInput = Player::Up;
						break;
					case sf::Keyboard::A:
						LastInput = Player::Left;
						break;
					case sf::Keyboard::S:
						LastInput = Player::Down;
						break;
					case sf::Keyboard::D:
						LastInput = Player::Right;
						break;
				}
				break;
			case sf::Event::Closed:
				WindowClosed = true;
				break;
		}
	}
}
