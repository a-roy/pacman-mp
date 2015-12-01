#version 130

#define TILE_PIXELS 8

uniform sampler2D fieldTexture;
uniform sampler2D eatenTexture;

void main()
{
	vec2 tile = vec2(floor(gl_TexCoord[0].st / TILE_PIXELS));
	float visibility = 1.0 - texture2D(eatenTexture, tile).r;
	gl_FragColor = texture2D(fieldTexture, gl_TexCoord[0].st);
}
