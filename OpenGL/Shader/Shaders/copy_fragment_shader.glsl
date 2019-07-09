#version 420 core

in vec3 outColor;
in vec2 textureCord;

uniform sampler2D gSampler;

void main(void)
{
	gl_FragColor = texture(gSampler, textureCord.st) * vec4(outColor, 1.0f);

	if(gl_FragColor.r >= 0.99f)
	{
		float xValue = sin(textureCord.s * 3.14f);
		float yValue = sin(textureCord.t * 3.14f);
		float combinedValue = (xValue + yValue) / 2.0f;
		gl_FragColor = vec4(combinedValue, 0.0f,0.0f,1.0f);
	}
	if (gl_FragColor.r >= 0.8f)
	{
		discard;
	}
	if(gl_FragColor.r <= 0.6f && gl_FragColor.b <= 0.5f)
	{
		discard;
	}
}