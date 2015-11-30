#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
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
	for (std::size_t i = 0; i < FIELD_WIDTH; i++)
	{
		for (std::size_t j = 0; j < FIELD_HEIGHT; j++)
		{
			int tx, ty, tw, th;
			float theta;
			bool flip;
			GetTile(field, i, j, tx, ty, tw, th, theta, flip);
		}
	}
	// TODO draw the field to SFData::FieldTexture
	// TODO update SFData::FieldShader
}

void Renderer::GetTile(Field *field, std::size_t x, std::size_t y,
		int &tx, int &ty, float &theta, bool &flip)
{
	if (field->Tiles[x][y] == Field::Wall)
	{
		tx = 19;
		ty = 120;
		theta = 0.f;
		flip = false;
	}
	// 2 edges, 2 walls -> type 0
	// 1 edge, 3 walls -> type 1 (orientation depends on diagonal)
	// 1 edge, 2 opposite walls -> type 2
	// 1 edge, 2 adjacent walls -> type 3
	// 1 edge, 1 wall -> type 2
	// no edge, 2 adjacent walls -> type 3
	// no edge, 2-3 opposite walls -> type 2
	// no edge, 4 walls -> type 3 (orientation depends on diagonal)
	// anything else -> undefined
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

static void DrawField(std::array<uint32_t, FIELD_HEIGHT> eaten)
{
	sf::Vector2f screenSize(SFData::Window->getSize());
	sf::Vector2f fieldSize(FIELD_WIDTH, FIELD_HEIGHT);
	sf::Vector2f offset((screenSize - fieldSize) / 2.f);
	sf::Vertex vertices[] =
	{
		sf::Vertex(
				offset,
				sf::Vector2f(0.f, 0.f)),
		sf::Vertex(
				sf::Vector2f(offset.x, offset.y + fieldSize.y),
				sf::Vector2f(0.f, fieldSize.y)),
		sf::Vertex(
				sf::Vector2f(offset.x + fieldSize.x, offset.y),
				sf::Vector2f(fieldSize.x, 0.f)),
		sf::Vertex(
				offset + fieldSize,
				fieldSize)
	};
	GLuint program = SFData::FieldShader.getNativeHandle();
	GLint location = glGetUniformLocation(program, "eaten");
	sf::Shader::bind(&SFData::FieldShader);
	glUniform1uiv(location, FIELD_HEIGHT, &eaten[0]);
	sf::RenderStates renderStates(&SFData::FieldShader);
	SFData::Window->draw(vertices, 4, sf::TrianglesStrip, renderStates);
	sf::Shader::bind(NULL);
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
