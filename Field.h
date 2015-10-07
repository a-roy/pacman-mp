//! \file
//! Field class declaration

#define FIELD_WIDTH  21
#define FIELD_HEIGHT 21
#define TILE_SIZE    6

#include <array>

class Field
{
public:
	enum TileType
	{
		Wall = 0x00,
		Empty = 0x01,
		Pellet = 0x03,
		PowerPellet = 0x05
	};

	std::array<std::array<TileType, FIELD_HEIGHT>, FIELD_WIDTH> Tiles;
};
