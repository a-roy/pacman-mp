#include "Renderer.h"

float Renderer::TileScale;
float Renderer::SpriteScale;

void Renderer::GetWallTile(Field *field, std::size_t x, std::size_t y,
		int &index, int &rotation, bool &flip)
{
	uint8_t neighborhood = 0x00;
	//uint8_t outercardinal = 0x00;
	uint8_t edges = 0x00;
	field->NeighborhoodWalls(x, y, neighborhood, edges);

	for (int i = 0; i < 4; i++)
	{
		if (((neighborhood & 0xD5) == 0x41)
				&& ((edges & 0x06) == 0x06))
		{
			index = 1;
			rotation = i;
			flip = false;
			return;
		}
		else if (neighborhood == 0x47)
		{
			index = 2;
			rotation = i;
			flip = false;
			return;
		}
		else if (neighborhood == 0x5C)
		{
			index = 2;
			rotation = i;
			flip = true;
			return;
		}
		else if ((neighborhood & 0x45) == 0x01)
		{
			index = 3;
			rotation = i;
			flip = false;
			if ((edges & 0x08) == 0x08)
			{
				rotation = (2 + i) % 4;
			}
			return;
		}
		else if ((neighborhood & 0x5F) == 0x1F)
		{
			index = 4;
			rotation = i;
			flip = false;
			return;
		}
		else if (((neighborhood & 0x55) == 0x14)
				&& ((edges & 0x09) == 0x00))
		{
			index = 5;
			rotation = i;
			flip = false;
			return;
		}
		else if (neighborhood == 0x7F)
		{
			index = 6;
			rotation = i;
			flip = false;
			return;
		}

		neighborhood = (neighborhood << 2) + (neighborhood >> 6);
		edges = ((edges << 1) + (edges >> 3)) & 0x0F;
	}

	index = 0;
	rotation = 0;
	flip = false;
}
