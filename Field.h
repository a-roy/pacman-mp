//! \file
//! Field class declaration

#pragma once

#define FIELD_WIDTH  28
#define FIELD_HEIGHT 31
#define TILE_SIZE    5
#define WALL_CHAR    '#'
#define EMPTY_CHAR   ' '
#define PELLET_CHAR  '.'
#define POWER_CHAR   'o'

#include <array>
#include <string>

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

		Field() { }
		Field(std::string file);
		TileType InterpolateAtPos(int x, int y) const;
};
