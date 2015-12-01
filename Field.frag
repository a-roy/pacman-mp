#define TILE_PIXELS 8

uniform sampler2D fieldTexture;
uniform sampler2D eatenTexture;

void main()
{
	uvec2 tile = uvec2(floor(gl_TexCoord[0].st / TILE_PIXELS));
	int visibility = 1 - texture2D(eatenTexture, tile);
	gl_FragColor = texture2D(fieldTexture, gl_TexCoord[0].st);
}
