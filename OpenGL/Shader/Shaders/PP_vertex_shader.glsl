#version 420 core

in vec3 a_position;
in vec2 a_texCord;

out vec2 textureCord;

void main(void)
{
	gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
	textureCord = a_texCord;
}