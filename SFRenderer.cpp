#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include "SFData.h"

void Renderer::CreateWindow(int width, int height, std::string title)
{
	SFData::Window = new sf::RenderWindow(sf::VideoMode(width, height), title);
	SFData::Window->setFramerateLimit(60);
	SFData::FieldTexture = new sf::RenderTexture();
	SFData::FieldTexture->create(
			FIELD_WIDTH * 8, FIELD_HEIGHT * 8);
	SFData::FieldShader.loadFromFile("../Field.vert", "../Field.frag");
	SFData::FieldShader.setParameter(
			"fieldTexture", SFData::FieldTexture->getTexture());
	SFData::PelletTexture.create(FIELD_WIDTH, FIELD_HEIGHT);
	SFData::FieldShader.setParameter(
			"eatenTexture", SFData::PelletTexture);
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

void Renderer::LoadField(Field *field, std::string texpath)
{
	sf::Vertex vertices[6 * FIELD_WIDTH * FIELD_HEIGHT];
	for (std::size_t i = 0; i < FIELD_WIDTH; i++)
	{
		for (std::size_t j = 0; j < FIELD_HEIGHT; j++)
		{
			int tile;
			int rotation;
			bool flip;
			if (field->Tiles[i][j] == Field::Wall)
			{
				GetWallTile(field, i, j, tile, rotation, flip);
			}
			else if (field->Tiles[i][j] == Field::GhostBox)
			{
				GetBoxTile(field, i, j, tile, rotation, flip);
			}
			else if (field->Tiles[i][j] == Field::GhostDoor)
			{
				tile = 8;
				rotation = 2;
				flip = false;
			}
			else if (field->Tiles[i][j] == Field::Pellet)
			{
				tile = 9;
				rotation = 0;
				flip = false;
			}
			else if (field->Tiles[i][j] == Field::PowerPellet)
			{
				tile = 10;
				rotation = 0;
				flip = false;
			}
			else
			{
				tile = 0;
				rotation = 0;
				flip = false;
			}
			// This is just how they are laid out in the spritesheet
			int x = 1 + 9 * tile;
			int y = 120;
			int w = 8;
			int h = 8;
			sf::Vector2f positions[4] =
			{
				sf::Vector2f(w * i, h * j),
				sf::Vector2f(w * i, h * (j + 1)),
				sf::Vector2f(w * (i + 1), h * (j + 1)),
				sf::Vector2f(w * (i + 1), h * j),
			};
			sf::Vector2f texcoords[4] =
			{
				sf::Vector2f(x, y),
				sf::Vector2f(x, y + h),
				sf::Vector2f(x + w, y + h),
				sf::Vector2f(x + w, y),
			};
			// Decide how to align the texture coordinates to the position
			// coordinates based on the flip and rotation parameters
			int step = flip ? -1 : 1;
			int start = flip ? (7 - rotation) : (4 + rotation);
			vertices[6 * (i * FIELD_HEIGHT + j)] =
				sf::Vertex(positions[0], texcoords[start % 4]);
			vertices[6 * (i * FIELD_HEIGHT + j) + 1] =
				sf::Vertex(positions[1], texcoords[(start + step) % 4]);
			vertices[6 * (i * FIELD_HEIGHT + j) + 2] =
				sf::Vertex(positions[2], texcoords[(start + step * 2) % 4]);
			vertices[6 * (i * FIELD_HEIGHT + j) + 3] =
				sf::Vertex(positions[0], texcoords[start % 4]);
			vertices[6 * (i * FIELD_HEIGHT + j) + 4] =
				sf::Vertex(positions[2], texcoords[(start + step * 2) % 4]);
			vertices[6 * (i * FIELD_HEIGHT + j) + 5] =
				sf::Vertex(positions[3], texcoords[(start + step * 3) % 4]);
		}
	}
	const sf::Texture &texture = SFData::GetTexture(texpath);
	SFData::FieldTexture->clear();
	SFData::FieldTexture->draw(
			vertices,
			6 * FIELD_WIDTH * FIELD_HEIGHT,
			sf::Triangles,
			sf::RenderStates(&texture));
	SFData::FieldTexture->display();
}

void Renderer::GetFieldPos(int &x, int &y)
{
	sf::Vector2u size = SFData::Window->getSize();
	x = (size.x - SpriteScale * 8 * FIELD_WIDTH) / 2;
	y = (size.y - SpriteScale * 8 * FIELD_HEIGHT) / 2;
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
	int fx, fy;
	GetFieldPos(fx, fy);
	sprite.setTextureRect(sf::IntRect(tx, ty, tw, th));
	sprite.setPosition(
			x * TileScale * 8 / TILE_SIZE + fx,
			y * TileScale * 8 / TILE_SIZE + fy);
	sprite.setRotation(theta);
	sprite.setScale(flip ? -SpriteScale : SpriteScale, SpriteScale);
	SFData::Window->draw(sprite);
}

void Renderer::DrawField(const Field::PelletStatus &pellets)
{
	sf::Vector2f screenSize(SFData::Window->getSize());
	sf::Vector2f fieldSize =
		8.f * sf::Vector2f(FIELD_WIDTH, FIELD_HEIGHT);
	sf::Vector2f scaledSize = SpriteScale * fieldSize;
	sf::Vector2f offset = (screenSize - scaledSize) / 2.f;
	sf::Vertex vertices[] =
	{
		sf::Vertex(
				offset,
				sf::Vector2f(0.f, 1.f)),
		sf::Vertex(
				sf::Vector2f(offset.x, offset.y + scaledSize.y),
				//sf::Vector2f(0.f, fieldSize.y)),
				sf::Vector2f(0.f, 0.f)),
		sf::Vertex(
				sf::Vector2f(offset.x + scaledSize.x, offset.y),
				//sf::Vector2f(fieldSize.x, 0.f)),
				sf::Vector2f(1.f, 1.f)),
		sf::Vertex(
				offset + scaledSize,
				//fieldSize)
				sf::Vector2f(1.f, 0.f))
	};
	sf::Image pellet_mask;
	pellet_mask.create(FIELD_WIDTH, FIELD_HEIGHT);
	for (std::size_t i = 0; i < FIELD_WIDTH; i++)
	{
		for (std::size_t j = 0; j < FIELD_HEIGHT; j++)
		{
			if (pellets.IsEaten(i, j))
			{
				pellet_mask.setPixel(i, j, sf::Color(255, 255, 255));
			}
		}
	}
	pellet_mask.flipVertically();
	SFData::PelletTexture.update(pellet_mask);
	sf::RenderStates renderStates(&SFData::FieldShader);
	SFData::Window->draw(vertices, 4, sf::TrianglesStrip, renderStates);
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
	delete SFData::FieldTexture;
}
