#version 460


layout (location=0) in vec3 position;
layout (location=2) in vec2 uvcoord;

uniform vec2 scaleFactrs;
out vec2 pos;


void main()
{

	pos = vec2(position.x*scaleFactrs.x,position.y*scaleFactrs.y);
	gl_Position = vec4(position.x*scaleFactrs.x,position.y*scaleFactrs.y,position.z,1.0);

}