#version 460

out vec4 fragColor;
uniform vec2 resolution;
uniform float tiles;
uniform sampler2D sampler;

subroutine vec4 TileColorType();
subroutine uniform TileColorType TileColor;

subroutine(TileColorType)
vec4 getTileColor()
{
	vec2 uv = gl_FragCoord.xy/resolution;
	vec2 gb = fract(uv*tiles);
	return vec4(0,gb,1.0);
}

subroutine(TileColorType)
vec4 getTileColorTex()
{
	vec2 uv = gl_FragCoord.xy/resolution;
	vec2 uvTile = fract(uv*tiles);
	return texture(sampler,uvTile);
}


void main()
{
	vec4 color = TileColor();

	fragColor = color;
}