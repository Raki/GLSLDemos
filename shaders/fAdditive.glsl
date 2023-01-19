#version 460


out vec4 fragColor;
in vec2 uvcoord_out;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

void main()
{
	fragColor = vec4(texture(sampler1,uvcoord_out).rgb+texture(sampler2,uvcoord_out).rgb,1.0);
}