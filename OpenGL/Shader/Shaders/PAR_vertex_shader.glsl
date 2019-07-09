#version 440 core

layout(location = 0) in vec4 position;
uniform mat4 vp;
uniform mat4 model;

out float lifetime;

void main(void)
{
	gl_Position = vp * model* vec4(position.xyz, 1.0f);
	lifetime = position.w;
}