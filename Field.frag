#version 130

uniform sampler2D fieldTexture;
uniform sampler2D eatenTexture;

void main()
{
	float visibility = 1.0 - texture2D(eatenTexture, gl_TexCoord[0].st).r;
	vec4 color = texture2D(fieldTexture, gl_TexCoord[0].st);
	gl_FragColor = vec4(color.rgb, visibility);
}
