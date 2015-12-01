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

void Renderer::LoadField(Field *field)
{
	// TODO draw the field to SFData::FieldTexture
	// TODO update SFData::FieldShader
}

void Renderer::GetTile(Field *field, std::size_t x, std::size_t y,
		int &tx, int &ty, int &rotation, bool &flip)
{
	uint8_t neighborhood = 0x00;
	uint8_t outercardinal = 0x00;
	field->NeighborhoodWalls(x, y, neighborhood, outercardinal);

	for (int i = 0; i < 4; i++)
	{
		// Tile 1
		if ((neighborhood & 0xD5) == 0x41)
		{
			tx = 10;
			ty = 120;
			rotation = i;
			flip = false;
			return;
		}
		// Tile 2
		else if ((neighborhood & 0xD7) == 0x47)
		{
			tx = 19;
			ty = 120;
			rotation = i;
			flip = false;
			return;
		}
		// Tile 2 (flipped)
		else if ((neighborhood & 0xBE) == 0x3A)
		{
			tx = 19;
			ty = 120;
			rotation = i;
			flip = true;
			return;
		}
		// Tile 3
		else if ((neighborhood & 0x45) == 0x01)
		{
			tx = 28;
			ty = 120;
			rotation = i;
			flip = false;
			return;
		}
		// Tile 4
		else if ((neighborhood & 0x5F) == 0x1F)
		{
			tx = 37;
			ty = 120;
			rotation = i;
			flip = false;
			return;
		}
		// Tile 5
		else if ((neighborhood & 0x5D) == 0x1C)
		{
			tx = 46;
			ty = 120;
			rotation = i;
			flip = false;
			return;
		}
		// Tile 6
		else if (neighborhood == 0x7F)
		{
			tx = 55;
			ty = 120;
			rotation = i;
			flip = false;
			return;
		}

		neighborhood = (neighborhood << 2) + (neighborhood >> 6);
		outercardinal = ((outercardinal << 1) + (outercardinal >> 3)) & 0xEF;
	}

	tx = 1;
	ty = 120;
	rotation = 0;
	flip = false;
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

void Renderer::DrawField(std::array<uint32_t, FIELD_HEIGHT> eaten)
{
	sf::Vertex vertices[4];
	// TODO initialize vertices
	// TODO set `eaten` uniform
	sf::RenderStates renderStates(&SFData::FieldShader);
	SFData::Window->draw(vertices, 4, sf::TrianglesStrip, renderStates);
	// TODO draw the saved texture to the screen using our shader
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
	unsigned int j = 0;
	for (unsigned int i = 0; i < len; i++)
	{
		const sf::Glyph &glyph = font.getGlyph(wtext[i], charSize, false);
		if (glyph.bounds.width > 0.f)
		{
			sf::Vertex topleft = sf::Vertex(
				sf::Vector2f(xi + glyph.bounds.left,
					y + glyph.bounds.top),
				sf::Vector2f(glyph.textureRect.left,
					glyph.textureRect.top));
			vertices[j++] = topleft;
			vertices[j++] = topleft;
			sf::Vertex botleft = sf::Vertex(
				sf::Vector2f(
					xi + glyph.bounds.left,
					y + glyph.bounds.top + glyph.bounds.height),
				sf::Vector2f(
					glyph.textureRect.left,
					glyph.textureRect.top + glyph.textureRect.height));
			vertices[j++] = botleft;
			sf::Vertex topright = sf::Vertex(
				sf::Vector2f(
					xi + glyph.bounds.left + glyph.bounds.width,
					y + glyph.bounds.top),
				sf::Vector2f(
					glyph.textureRect.left + glyph.textureRect.width,
					glyph.textureRect.top));
			vertices[j++] = topright;
			sf::Vertex botright = sf::Vertex(
				sf::Vector2f(
					xi + glyph.bounds.left + glyph.bounds.width,
					y + glyph.bounds.top + glyph.bounds.height),
				sf::Vector2f(
					glyph.textureRect.left + glyph.textureRect.width,
					glyph.textureRect.top + glyph.textureRect.height));
			vertices[j++] = botright;
			vertices[j++] = botright;
		}
		xi += glyph.advance;
	}
	SFData::Window->draw(
			vertices + 1,
			j - 2,
			sf::TrianglesStrip,
			sf::RenderStates(&texture));
	free(vertices);
}

void Renderer::Deinit()
{
	SFData::Window->close();
	delete SFData::Window;
}
