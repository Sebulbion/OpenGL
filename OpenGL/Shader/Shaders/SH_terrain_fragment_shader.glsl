#version 420 core

in float isGrass;

void main(void)
{
	if (isGrass != 0.0f)
	{
		discard;
	}
}