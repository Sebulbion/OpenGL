#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

 uniform mat4 gScale;
 uniform mat4 gRotate;
 uniform mat4 gTranslate;
 uniform mat4 gView;
 //uniform mat4 gOrtho;
 uniform mat4 gPerspective;

out vec3 outColor;
out vec2 TexCoords;

void main(void)
{
	gl_Position = gPerspective * gView * gTranslate * gRotate * gScale * vec4(position, 1.0f);
	TexCoords = texCoords;
}