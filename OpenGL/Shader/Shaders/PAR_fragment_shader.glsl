#version 440 core

out vec4 color;
in float lifetime;

void main(void)
{
	color = vec4(0.9f, 0.9f, 0.9f, lifetime);
}