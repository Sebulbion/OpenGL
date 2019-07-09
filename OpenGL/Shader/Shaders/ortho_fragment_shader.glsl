#version 420 core

in vec3 outColor;
in vec2 textureCord;

uniform float currentTime;
uniform sampler2D gSampler;

vec2 editedTextureCord;
mat2 rotationMatrix;

void main(void)
{
	rotationMatrix = mat2(
			cos(currentTime),	-sin(currentTime),
			sin(currentTime),	cos(currentTime));

	editedTextureCord = textureCord;
	editedTextureCord *= 2;
	editedTextureCord -= 1;
	if(abs(length(editedTextureCord)) > 1)
	{
		discard;
	}
	editedTextureCord *= rotationMatrix;
	editedTextureCord += 1;
	editedTextureCord /= 2;

	gl_FragColor = texture(gSampler, editedTextureCord.st) * vec4(outColor, 1.0f);
}