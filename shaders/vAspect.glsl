#version 460


layout (location=0) in vec3 position;
//layout (location=1) in vec3 normal;
layout (location=2) in vec2 uvcoord;

uniform vec2 scaleFactrs;
out vec2 uvcoord_out;


void main()
{

	uvcoord_out = uvcoord;
	gl_Position = vec4(position.x*scaleFactrs.x,position.y*scaleFactrs.y,position.z,1.0);

}