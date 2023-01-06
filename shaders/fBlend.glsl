#version 460


out vec4 fragColor;
in vec2 uvcoord_out;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

uniform float blendFactor;

void main()
{
	fragColor = (1.-blendFactor)*texture(sampler1,uvcoord_out)+(blendFactor*texture(sampler2,uvcoord_out));
}