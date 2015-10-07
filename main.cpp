//! \file
//! Entry point

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "SFAnimation.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "My window");
	window.setFramerateLimit(60);
	sf::CircleShape shape(20.f);
	shape.setFillColor(sf::Color::Yellow);

	sf::Texture texture;
	if (!texture.loadFromFile("../pacman.png"))
	{
		return 1;
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale(sf::Vector2f(-2.0f, 2.0f));
	sprite.setOrigin(sf::Vector2f(16, 16));

	SFAnimation pac_move(4);
	pac_move.Frames.push_back(sf::IntRect(34, 170, 32, 32));
	pac_move.Frames.push_back(sf::IntRect(1, 170, 32, 32));
	pac_move.Frames.push_back(sf::IntRect(34, 170, 32, 32));
	pac_move.Frames.push_back(sf::IntRect(67, 170, 32, 32));
	int frame = 0;

	Field f;
	for (int i = 0; i < FIELD_WIDTH; i++)
	{
		for (int j = 0; j < FIELD_HEIGHT; j++)
		{
			if (i == 0 || i == FIELD_WIDTH - 1 || j == 0 || j == FIELD_HEIGHT - 1)
			{
				f.Tiles[i][j] = Field::Wall;
			}
			else
			{
				f.Tiles[i][j] = Field::Empty;
			}
		}
	}
	Game g(f, {{ Player(Player::Pacman) }});

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			switch (event.type)
			{
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
						case sf::Keyboard::W:
							g.Players[0].NextDir = Player::Up;
							break;
						case sf::Keyboard::A:
							g.Players[0].NextDir = Player::Left;
							break;
						case sf::Keyboard::S:
							g.Players[0].NextDir = Player::Down;
							break;
						case sf::Keyboard::D:
							g.Players[0].NextDir = Player::Right;
							break;
					}
					break;
				case sf::Event::Closed:
					window.close();
					break;
			}
        }

		g.update();
		sprite.setTextureRect(pac_move.getRect(frame++));
		sprite.setPosition(g.Players[0].XPos * 8.f, g.Players[0].YPos * 8.f);
		sprite.setRotation(g.Players[0].CurrentDir * -90);
        window.clear();

		// draw
		window.draw(sprite);

        window.display();
    }

    return 0;
}
