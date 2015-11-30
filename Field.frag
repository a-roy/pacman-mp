#version 330 core

#define FIELD_HEIGHT 31
#define TILE_PIXELS 8

uniform sampler2D fieldTexture;
uniform uint eaten[FIELD_HEIGHT];

void main()
{
	uvec2 tile(floor(gl_TexCoord[0].xy / TILE_PIXELS));
	uint eaten_row = eaten[tile.y];
	int eaten_tile = (eaten_row & (1U << tile.x)) >> tile.x;
	int visibility = 1 - eaten_tile;
	gl_FragColor = texture2D(fieldTexture, gl_TexCoord[0].xy) * visibility;
}
