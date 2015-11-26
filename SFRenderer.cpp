#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include "SFData.h"

float Renderer::TileScale;
float Renderer::SpriteScale;

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
	sprite.setScale(sf::Vector2f(SpriteScale, SpriteScale));
	sprite.setOrigin(sf::Vector2f(8, 8));

	int index = SFData::Sprites.size();
	SFData::Sprites.push_back(sprite);
	return index;
}

int Renderer::LoadFont(std::string fontpath)
{
	sf::Font font;
	font.loadFromFile(fontpath);
	int index = SFData::Fonts.size();
	SFData::Fonts.push_back(font);
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

void Renderer::DrawSprite(const Sprite &s, int x, int y, float theta,
		bool flip, int anim, int frame)
{
	sf::Sprite &sprite = SFData::Sprites[s.Index];
	int tx, ty, tw, th;
	s.Animations[anim].GetRect(frame, tx, ty, tw, th);
	sprite.setTextureRect(sf::IntRect(tx, ty, tw, th));
	sprite.setPosition(x * TileScale, y * TileScale);
	sprite.setRotation(theta);
	sprite.setScale(flip ? -SpriteScale : SpriteScale, SpriteScale);
	SFData::Window->draw(sprite);
}

void Renderer::DrawText(int fontIndex, std::string text,
		unsigned int charSize, int x, int y)
{
	const sf::Font &font = SFData::Fonts[fontIndex];
	const sf::Texture &texture = font.getTexture(charSize);
	float xi = (float)x;
	std::wstring wtext;
	wtext.assign(text.begin(), text.end());
	std::size_t len = wtext.size();
	sf::Vertex *vertices = (sf::Vertex *)malloc(len * 6 * sizeof(sf::Vertex));
	for (unsigned int i = 0; i < len; i++)
	{
		const sf::Glyph &glyph = font.getGlyph(wtext[i], charSize, false);
		sf::Vertex topleft = sf::Vertex(
				sf::Vector2f(xi + glyph.bounds.left, y + glyph.bounds.top),
				sf::Vector2f(glyph.textureRect.left, glyph.textureRect.top));
		sf::Vertex topright = sf::Vertex(
				sf::Vector2f(
					xi + glyph.bounds.left + glyph.bounds.width,
					y + glyph.bounds.top),
				sf::Vector2f(
					glyph.textureRect.left + glyph.textureRect.width,
					glyph.textureRect.top));
		sf::Vertex botleft = sf::Vertex(
				sf::Vector2f(
					xi + glyph.bounds.left,
					y + glyph.bounds.top + glyph.bounds.height),
				sf::Vector2f(
					glyph.textureRect.left,
					glyph.textureRect.top + glyph.textureRect.height));
		sf::Vertex botright = sf::Vertex(
				sf::Vector2f(
					xi + glyph.bounds.left + glyph.bounds.width,
					y + glyph.bounds.top + glyph.bounds.height),
				sf::Vector2f(
					glyph.textureRect.left + glyph.textureRect.width,
					glyph.textureRect.top + glyph.textureRect.height));
		vertices[0 + 6 * i] = topleft;
		vertices[1 + 6 * i] = topleft;
		vertices[2 + 6 * i] = botleft;
		vertices[3 + 6 * i] = topright;
		vertices[4 + 6 * i] = botright;
		vertices[5 + 6 * i] = botright;
		SFData::Window->draw(
				vertices, 4, sf::TrianglesStrip, sf::RenderStates(&texture));
		xi += glyph.advance;
	}
	SFData::Window->draw(
			vertices + 1,
			len * 6 - 2,
			sf::TrianglesStrip,
			sf::RenderStates(&texture));
	free(vertices);
}

void Renderer::Deinit()
{
	SFData::Window->close();
	delete SFData::Window;
}
