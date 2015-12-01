#include "Renderer.h"

void Renderer::GetTile(Field *field, std::size_t x, std::size_t y,
		int &index, int &rotation, bool &flip)
{
	uint8_t neighborhood = 0x00;
	uint8_t outercardinal = 0x00;
	field->NeighborhoodWalls(x, y, neighborhood, outercardinal);

	for (int i = 0; i < 4; i++)
	{
		if ((neighborhood & 0xD5) == 0x41)
		{
			index = 1;
			rotation = i;
			flip = false;
			return;
		}
		else if ((neighborhood & 0xD7) == 0x47)
		{
			index = 2;
			rotation = i;
			flip = false;
			return;
		}
		else if ((neighborhood & 0xBE) == 0x3A)
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
			return;
		}
		else if ((neighborhood & 0x5F) == 0x1F)
		{
			index = 4;
			rotation = i;
			flip = false;
			return;
		}
		else if ((neighborhood & 0x5D) == 0x1C)
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
		outercardinal = ((outercardinal << 1) + (outercardinal >> 3)) & 0xEF;
	}

	index = 0;
	rotation = 0;
	flip = false;
}
