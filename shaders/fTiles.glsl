#version 460

out vec4 fragColor;
uniform vec2 resolution;
uniform float tiles;

void main()
{
	vec2 uv = gl_FragCoord.xy/resolution;
	vec2 rg = fract(uv*tiles);

	fragColor = vec4(rg,0.,0.);
}