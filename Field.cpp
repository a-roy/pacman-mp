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
	int w = FIELD_WIDTH * TILE_SIZE;
	int h = FIELD_HEIGHT * TILE_SIZE;
	int del = (TILE_SIZE - 1) / 2;
	std::size_t x_c = (x + w) % w;
	std::size_t x_r = (x_c + del) % w;
	std::size_t x_l = (x_c - del + w) % w;
	std::size_t y_c = (y + h) % h;
	std::size_t y_d = (y_c + del) % h;
	std::size_t y_u = (y_c - del + h) % h;
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
