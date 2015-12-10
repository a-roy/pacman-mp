//! \file
//! Field class declaration

#pragma once

#define FIELD_WIDTH  28
#define FIELD_HEIGHT 31
#define TILE_SIZE    60
#define WALL_CHAR    '#'
#define EMPTY_CHAR   ' '
#define EDGE_CHAR    '/'
#define PELLET_CHAR  '.'
#define POWER_CHAR   'o'
#define GBOX_CHAR    '='
#define GZONE_CHAR   '~'
#define GDOOR_CHAR   '%'
#define TUNNEL_CHAR  '$'

#include <cstdint>
#include <array>
#include <string>
#include "Position.h"

class Field
{
	public:
		// 0x01 Pac-Man can move
		// 0x02 Ghost can move
		// 0x04 Food
		// 0x08 Special
		enum TileType
		{
			Wall = 0x00,
			PacmanZone = 0x01,
			GhostBox = 0x08,
			GhostZone = 0x02,
			GhostDoor = 0x0A,
			Empty = 0x03,
			Tunnel = 0x0B,
			Pellet = 0x07,
			PowerPellet = 0x0F,
			Edge = 0x10
		};
		class PelletStatus
		{
			public:
				std::array<uint32_t, FIELD_HEIGHT> Bitfield;

				PelletStatus() { Bitfield.fill(0x00000000); }
				bool IsEaten(int x, int y) const
				{ return (Bitfield[y] & (1U << x)) != 0U; }
				void Eat(int x, int y) { Bitfield[y] |= (1U << x); }
				bool operator==(const PelletStatus &other) const
				{
					bool equal = true;
					for (std::size_t i = 0; i < FIELD_HEIGHT; i++)
					{
						equal = equal && (Bitfield[i] == other.Bitfield[i]);
					}
					return equal;
				}
		};

		std::array<std::array<TileType, FIELD_HEIGHT>, FIELD_WIDTH> Tiles;

		Field() { }
		Field(std::string file);
		TileType InterpolateAtPos(Position pos) const;
		void NeighborhoodInfo(
				std::size_t x, std::size_t y, TileType wall, TileType edge,
				uint8_t &neighborhood, uint8_t &outercardinal) const;
};
