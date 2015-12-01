#include "Field.h"
#include <iostream>
#include <fstream>

Field::Field(std::string file)
{
	std::ifstream fs;
	fs.open(file);
	if (fs.is_open())
	{
		std::string line;
		for (int i = 0; i < FIELD_HEIGHT && getline(fs, line); i++)
		{
			for (unsigned int j = 0; j < FIELD_WIDTH && j < line.size(); j++)
			{
				if (line[j] == WALL_CHAR)
				{
					Tiles[j][i] = Wall;
				}
				else if (line[j] == EMPTY_CHAR)
				{
					Tiles[j][i] = Empty;
				}
				else if (line[j] == EDGE_CHAR)
				{
					Tiles[j][i] = Edge;
				}
				else if (line[j] == PELLET_CHAR)
				{
					Tiles[j][i] = Pellet;
				}
				else if (line[j] == POWER_CHAR)
				{
					Tiles[j][i] = PowerPellet;
				}
				else if (line[j] == GBOX_CHAR)
				{
					Tiles[j][i] = GhostBox;
				}
				else if (line[j] == GZONE_CHAR)
				{
					Tiles[j][i] = GhostZone;
				}
				else if (line[j] == GDOOR_CHAR)
				{
					Tiles[j][i] = GhostDoor;
				}
			}
		}
		fs.close();
	}
}

Field::TileType Field::InterpolateAtPos(int x, int y) const
{
	// Looks confusing, but all it's doing is checking a half-tile neighborhood
	// of the given position and doing a bitwise AND (so walls have priority
	// over open space).
	int w = FIELD_WIDTH * TILE_SIZE;
	int h = FIELD_HEIGHT * TILE_SIZE;
	int d_sm = (TILE_SIZE - 1) / 2;
	int d_lg = TILE_SIZE / 2;
	std::size_t x_c = (x + w) % w;
	std::size_t x_r = (x_c + d_lg) % w;
	std::size_t x_l = (x_c - d_sm + w) % w;
	std::size_t y_c = (y + h) % h;
	std::size_t y_d = (y_c + d_lg) % h;
	std::size_t y_u = (y_c - d_sm + h) % h;
	TileType t5 = Tiles[x_c / TILE_SIZE][y_c / TILE_SIZE];
	TileType t6 = Tiles[x_r / TILE_SIZE][y_c / TILE_SIZE];
	TileType t9 = Tiles[x_r / TILE_SIZE][y_u / TILE_SIZE];
	TileType t8 = Tiles[x_c / TILE_SIZE][y_u / TILE_SIZE];
	TileType t7 = Tiles[x_l / TILE_SIZE][y_u / TILE_SIZE];
	TileType t4 = Tiles[x_l / TILE_SIZE][y_c / TILE_SIZE];
	TileType t1 = Tiles[x_l / TILE_SIZE][y_d / TILE_SIZE];
	TileType t2 = Tiles[x_c / TILE_SIZE][y_d / TILE_SIZE];
	TileType t3 = Tiles[x_r / TILE_SIZE][y_d / TILE_SIZE];
	return (TileType)(t5 & t6 & t9 & t8 & t7 & t4 & t1 & t2 & t3);
}

void Field::NeighborhoodWalls(
		std::size_t x, std::size_t y,
		uint8_t &neighborhood, uint8_t &edges) const
{
	neighborhood = 0x00;
	edges = 0x00;
	if (x < FIELD_WIDTH - 1 && y < FIELD_HEIGHT - 1
			&& Tiles[x + 1][y + 1] == Wall)
	{
		neighborhood += 1;
	}
	neighborhood <<= 1;
	if (y < FIELD_HEIGHT - 1 && Tiles[x][y + 1] == Wall)
	{
		neighborhood += 1;
	}
	neighborhood <<= 1;
	if (y == FIELD_HEIGHT - 1 || Tiles[x][y + 1] == Edge)
	{
		edges += 1;
	}
	edges <<= 1;
	if (x > 0 && y < FIELD_HEIGHT - 1
			&& Tiles[x - 1][y + 1] == Wall)
	{
		neighborhood += 1;
	}
	neighborhood <<= 1;
	if (x > 0 && Tiles[x - 1][y] == Wall)
	{
		neighborhood += 1;
	}
	neighborhood <<= 1;
	if (x == 0 || Tiles[x - 1][y] == Edge)
	{
		edges += 1;
	}
	edges <<= 1;
	if (x > 0 && y > 0 && Tiles[x - 1][y - 1] == Wall)
	{
		neighborhood += 1;
	}
	neighborhood <<= 1;
	if (y > 0 && Tiles[x][y - 1] == Wall)
	{
		neighborhood += 1;
	}
	neighborhood <<= 1;
	if (y == 0 || Tiles[x][y - 1] == Edge)
	{
		edges += 1;
	}
	edges <<= 1;
	if (x < FIELD_WIDTH - 1 && y > 0
			&& Tiles[x + 1][y - 1] == Wall)
	{
		neighborhood += 1;
	}
	neighborhood <<= 1;
	if (x < FIELD_WIDTH - 1 && Tiles[x + 1][y] == Wall)
	{
		neighborhood += 1;
	}
	if (x == FIELD_WIDTH - 1 || Tiles[x + 1][y] == Edge)
	{
		edges += 1;
	}
}
