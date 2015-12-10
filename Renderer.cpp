#include "Renderer.h"

float Renderer::TileScale;
float Renderer::SpriteScale;

#define NB_N  0x00
#define NB_R  0x01
#define NB_UR 0x02
#define NB_U  0x04
#define NB_UL 0x08
#define NB_L  0x10
#define NB_DL 0x20
#define NB_D  0x40
#define NB_DR 0x80
#define ED_N  0x00
#define ED_R  0x01
#define ED_U  0x02
#define ED_L  0x04
#define ED_D  0x08

void Renderer::GetWallTile(Field *field, std::size_t x, std::size_t y,
		int &index, int &rotation, bool &flip)
{
	uint8_t neighborhood = 0x00;
	uint8_t edges = 0x00;
	field->NeighborhoodInfo(
			x, y, Field::Wall, Field::Edge, neighborhood, edges);

	for (int i = 0; i < 4; i++)
	{
		if (((neighborhood & (NB_R|NB_U|NB_L|NB_D|NB_DR)) == (NB_R|NB_D))
				&& ((edges & (ED_U|ED_L)) == (ED_U|ED_L)))
		{
			index = 1;
			rotation = i;
			flip = false;
			return;
		}
		else if (neighborhood == (NB_R|NB_UR|NB_U|NB_D))
		{
			index = 2;
			rotation = i;
			flip = false;
			return;
		}
		else if (neighborhood == (NB_U|NB_UL|NB_L|NB_D))
		{
			index = 2;
			rotation = i;
			flip = true;
			return;
		}
		else if ((neighborhood & (NB_R|NB_U|NB_D)) == NB_R)
		{
			index = 3;
			rotation = i;
			flip = false;
			if ((edges & ED_D) == ED_D)
			{
				rotation = (2 + i) % 4;
			}
			return;
		}
		else if ((neighborhood & (NB_R|NB_UR|NB_L|NB_UL|NB_L|NB_D))
				== (NB_R|NB_UR|NB_L|NB_UL|NB_L))
		{
			index = 4;
			rotation = i;
			flip = false;
			return;
		}
		else if (((neighborhood & (NB_R|NB_U|NB_L|NB_D)) == (NB_U|NB_L))
				&& ((edges & (ED_R|ED_D)) == ED_N))
		{
			index = 5;
			rotation = i;
			flip = false;
			return;
		}
		else if (neighborhood == (NB_R|NB_UR|NB_U|NB_UL|NB_L|NB_DL|NB_D))
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

void Renderer::GetBoxTile(Field *field, std::size_t x, std::size_t y,
		int &index, int &rotation, bool &flip)
{
	uint8_t neighborhood;
	uint8_t edges;
	field->NeighborhoodInfo(
			x, y, Field::GhostBox, Field::GhostZone, neighborhood, edges);
	for (int i = 0; i < 4; i++)
	{
		if (((neighborhood & (NB_R|NB_U|NB_L|NB_D)) == (NB_U|NB_L))
				&& ((edges & (ED_R|ED_D)) == (ED_N)))
		{
			index = 7;
			rotation = i;
			flip = false;
			return;
		}
		else if ((neighborhood & (NB_R|NB_U|NB_D)) == NB_R)
		{
			index = 3;
			rotation = i;
			flip = false;
			if ((edges & ED_D) == ED_D)
			{
				rotation = (2 + i) % 4;
			}
			return;
		}
		neighborhood = (neighborhood << 2) + (neighborhood >> 6);
		edges = ((edges << 1) + (edges >> 3)) & 0x0F;
	}
}
