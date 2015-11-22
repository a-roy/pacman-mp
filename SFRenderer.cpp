#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include "SFData.h"

float Renderer::Scale;

void Renderer::CreateWindow(int width, int height, std::string title)
{
	SFData::Window = new sf::RenderWindow(sf::VideoMode(width, height), title);
	SFData::Window->setFramerateLimit(60);
}

bool Renderer::WindowOpen()
{
	return SFData::Window->isOpen();
}

int Renderer::CreateSprite(std::string texpath)
{
	sf::Sprite sprite;
	sprite.setTexture(SFData::GetTexture(texpath));
	sprite.setScale(sf::Vector2f(-2.0f, 2.0f));
	sprite.setOrigin(sf::Vector2f(16, 16));

	int index = SFData::Sprites.size();
	SFData::Sprites.push_back(sprite);
	return index;
}

int Renderer::CreateText(std::string fontpath, std::string string, int size)
{
	sf::Text text;
	text.setFont(SFData::GetFont(fontpath));
	text.setString(string);
	text.setCharacterSize(size);
	text.setColor(sf::Color::White);

	int index = SFData::Texts.size();
	SFData::Texts.push_back(text);
	return index;
}

void Renderer::Clear()
{
	SFData::Window->clear();
}

void Renderer::Display()
{
	SFData::Window->display();
}

void Renderer::DrawSprite(const Sprite &s, int x, int y, float theta, int anim, int frame)
{
	sf::Sprite &sprite = SFData::Sprites[s.Index];
	int tx, ty, tw, th;
	s.Animations[anim].GetRect(frame, tx, ty, tw, th);
	sprite.setTextureRect(sf::IntRect(tx, ty, tw, th));
	sprite.setPosition(x * Scale, y * Scale);
	sprite.setRotation(theta);
	SFData::Window->draw(sprite);
}

void Renderer::DrawText(int t, int x, int y)
{
	sf::Text &text = SFData::Texts[t];
	text.setPosition(x * Scale, y * Scale);
	SFData::Window->draw(text);
}

void Renderer::Deinit()
{
	SFData::Window->close();
	delete SFData::Window;
}
