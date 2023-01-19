#version 460


out vec4 fragColor;
in vec2 uvcoord_out;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

uniform float blendFactor;

subroutine vec4 GetColorType();
subroutine uniform GetColorType GetColor;

subroutine(GetColorType)
vec4 getBlendColor()
{
	return (1.-blendFactor)*texture(sampler1,uvcoord_out)+(blendFactor*texture(sampler2,uvcoord_out));
}

subroutine(GetColorType)
vec4 getDiffColor()
{
	vec4 color1= texture(sampler1,uvcoord_out);
	vec4 color2= texture(sampler2,uvcoord_out);
	return vec4(abs(color1.r-color2.r),abs(color1.g-color2.g),abs(color1.b-color2.b),1);
}

void main()
{
	fragColor = GetColor();
}