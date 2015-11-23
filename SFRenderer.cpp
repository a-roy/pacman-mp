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
	sprite.setOrigin(sf::Vector2f(8, 8));

	int index = SFData::Sprites.size();
	SFData::Sprites.push_back(sprite);
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

void Renderer::DrawText(std::string fontpath, std::string text,
		unsigned int charSize, int x, int y)
{
	const sf::Font &font = SFData::GetFont(fontpath);
	const sf::Texture &texture = font.getTexture(charSize);
	float xi = (float)x;
	std::wstring wtext;
	wtext.assign(text.begin(), text.end());
	for (unsigned int i = 0, size = wtext.size(); i < size; i++)
	{
		const sf::Glyph &glyph = font.getGlyph(wtext[i], charSize, false);
		sf::Vertex vertices[] =
		{
			sf::Vertex(
					sf::Vector2f(
						xi + glyph.bounds.left,
						y + glyph.bounds.top),
					sf::Vector2f(
						glyph.textureRect.left,
						glyph.textureRect.top)),
			sf::Vertex(
					sf::Vector2f(
						xi + glyph.bounds.left + glyph.bounds.width,
						y + glyph.bounds.top),
					sf::Vector2f(
							glyph.textureRect.left + glyph.textureRect.width,
							glyph.textureRect.top)),
			sf::Vertex(
					sf::Vector2f(
						xi + glyph.bounds.left,
						y + glyph.bounds.top + glyph.bounds.height),
					sf::Vector2f(
							glyph.textureRect.left,
							glyph.textureRect.top + glyph.textureRect.height)),
			sf::Vertex(
					sf::Vector2f(
						xi + glyph.bounds.left + glyph.bounds.width,
						y + glyph.bounds.top + glyph.bounds.height),
					sf::Vector2f(
							glyph.textureRect.left + glyph.textureRect.width,
							glyph.textureRect.top + glyph.textureRect.height)),
		};
		SFData::Window->draw(
				vertices, 4, sf::TrianglesStrip, sf::RenderStates(&texture));
		xi += glyph.advance;
	}
}

void Renderer::Deinit()
{
	SFData::Window->close();
	delete SFData::Window;
}
