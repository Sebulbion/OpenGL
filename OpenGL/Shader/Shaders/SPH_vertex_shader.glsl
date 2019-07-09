#version 420 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

 uniform mat4 model;
 uniform mat4 vp;
 uniform mat4 lightVp;

out vec3 outColor;
out vec3 fragNormal;
out vec4 FragPosLightSpace;

void main(void)
{
	gl_Position = vp * model * vec4(position, 1.0f);
	FragPosLightSpace = lightVp * model * vec4(position, 1.0f);
	fragNormal = normal;
	outColor = vec3(1.0f, 1.0f, 0.0f);
}