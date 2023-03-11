#version 460


out vec4 fragColor;
in vec2 uvcoord_out;
uniform sampler2D sampler1;
uniform sampler2D sampler2;

uniform float blendFactor;

void main()
{
	vec4 srcColor = texture(sampler1,uvcoord_out);
	vec4 dstColor = texture(sampler2,uvcoord_out);

	
	float final_alpha = srcColor.a + dstColor.a * (1.0 - srcColor.a);
    fragColor = vec4(
        (srcColor.rgb * srcColor.a + dstColor.rgb * dstColor.a * (1.0 - srcColor.a)) / final_alpha,
        final_alpha
    );

	//fragColor = vec4(srcColor.rgb*srcColor.a+dstColor.rgb*dstColor.a,1.);

	//fragColor = (1.-srcColor.a)*dstColor+(srcColor.a*srcColor);
}