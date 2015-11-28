#include "Field.h"
#include <iostream>
#include <fstream>

Field(std::string file)
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
			}
		}
		fs.close();
	}
}
