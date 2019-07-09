#version 430 core

uniform mat4 model;
uniform mat4 vp; 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

void main(void)
{
	gl_Position =  vp * model * vec4(position, 1.0f);
}