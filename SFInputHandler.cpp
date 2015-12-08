#include "InputHandler.h"
#include "SFML/Window.hpp"
#include "SFData.h"

Position InputHandler::LastInput;
int InputHandler::InputTime;
bool InputHandler::WindowClosed = false;

void InputHandler::PollEvents()
{
    sf::Event event;
    while (SFData::Window->pollEvent(event))
    {
		Position newInput(0, 0);
		switch (event.type)
		{
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
					case sf::Keyboard::W:
						newInput = Up;
						break;
					case sf::Keyboard::Up:
						newInput = Up;
						break;
					case sf::Keyboard::A:
						newInput = Left;
						break;
					case sf::Keyboard::Left:
						newInput = Left;
						break;
					case sf::Keyboard::S:
						newInput = Down;
						break;
					case sf::Keyboard::Down:
						newInput = Down;
						break;
					case sf::Keyboard::D:
						newInput = Right;
						break;
					case sf::Keyboard::Right:
						newInput = Right;
						break;
					default:
						continue;
				}
				LastInput = newInput;
				InputTime = -1;
				break;
			case sf::Event::JoystickMoved:
				switch (event.joystickMove.axis)
				{
					case sf::Joystick::PovX:
						if (event.joystickMove.position > 0.f)
						{
							newInput = Right;
						}
						else if (event.joystickMove.position < 0.f)
						{
							newInput = Left;
						}
						break;
					case sf::Joystick::PovY:
						if (event.joystickMove.position > 0.f)
						{
							newInput = Up;
						}
						else if (event.joystickMove.position < 0.f)
						{
							newInput = Down;
						}
						break;
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
