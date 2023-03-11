#version 460


out vec4 fragColor;
in vec2 uvcoord_out;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

uniform float blendFactor;

subroutine vec4 BlendAlphaType(vec4 srcColor,vec4 dstColor);
subroutine uniform BlendAlphaType BlendAlpha;

subroutine(BlendAlphaType)
vec4 defaultBlend(vec4 srcColor,vec4 dstColor)
{
	return  (1.-srcColor.a)*dstColor+(srcColor.a*srcColor);
}

subroutine(BlendAlphaType)
vec4 preMulBlendBasic(vec4 srcColor,vec4 dstColor)
{
	return vec4(srcColor.rgb*srcColor.a+dstColor.rgb*dstColor.a,1.);
}

subroutine(BlendAlphaType)
vec4 preMulBlend(vec4 srcColor,vec4 dstColor)
{
	float final_alpha = srcColor.a + dstColor.a * (1.0 - srcColor.a);
	return vec4(
        (srcColor.rgb * srcColor.a + dstColor.rgb * dstColor.a * (1.0 - srcColor.a)) / final_alpha,
        final_alpha
    );
}

void main()
{
	vec4 srcColor = texture(sampler1,uvcoord_out);
	vec4 dstColor = texture(sampler2,uvcoord_out);

	fragColor = BlendAlpha(srcColor,dstColor);
}