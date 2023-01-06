#version 460


out vec4 fragColor;
in vec2 uvcoord_out;
uniform sampler2D sampler;

uniform vec3 color;

subroutine vec4 GetColorType();
subroutine uniform GetColorType GetColor;

subroutine(GetColorType)
vec4 getColor()
{
	return vec4(color,1.);
}

subroutine(GetColorType)
vec4 getTextureColor()
{
	return texture(sampler,uvcoord_out);
}


void main()
{
	fragColor = GetColor();
}