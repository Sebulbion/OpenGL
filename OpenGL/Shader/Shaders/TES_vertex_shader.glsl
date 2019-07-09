#version 430 core

in vec3 a_position;
in vec3 a_Color;
out vec3 tcsColor;
in vec2 a_texCord;
out vec2 tcsTexCord;

//uniform mat4 mvp;

void main(){
	gl_Position = vec4(a_position, 1.0f);
	tcsColor = a_Color;
	tcsTexCord = a_texCord;
}