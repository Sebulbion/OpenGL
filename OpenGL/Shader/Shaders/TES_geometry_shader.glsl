#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices= 15) out;

in VS_GS_VERTEX{
	in vec4 position;
	in vec4 FragPosLightSpace;
	in vec4 color;
	in vec2 texCord;
	in vec3 worldPosition;
}gs_in[];

out vec4 outColor;
out vec2 textureCord;
out vec4 FragPosLightSpace;
out float isGrass;
//out vec3 normal;

uniform sampler2D gSampler;
uniform float currentTime;
uniform mat4 mvp;

float Random(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff;
	return 1.0f - float(t) * 0.931322574615478515625e-9;
}

float Smooth(float x, float y)
{
	float corners = (Random(int(x - 1.0f), int(y - 1.0f)) + Random(int(x + 1.0f), int(y - 1.0f)) + Random(int(x - 1.0f), int(y + 1.0f)) + Random(int(x + 1.0f), int(y + 1.0f))) / 16.0f;
	float sides = (Random(int(x - 1.0f), int(y)) + Random(int(x + 1.0f), int(y)) + Random(int(x), int(y - 1.0f)) + Random(int(x), int(y + 1.0f))) / 8.0f;
	float center = Random(int(x), int(y)) / 4.0f;
	return corners + sides + center;
}

float Interpolate(float a, float b, float x)
{
	//float ft = x * 3.1415927;
	//float f = (1 - cos(ft)) * 0.5f;
	//return a*(1 - f) + b*f;

	return a + x * (b - a);
}

float Noise(float x, float y)
{
	float	fractional_X = x - int(x);
	float 	fractional_Y = y - int(y);

	//smooths
	float v1 = Smooth(int(x), int(y));
	float v2 = Smooth(int(x) + 1, int(y));
	float v3 = Smooth(int(x), int(y) + 1);
	float v4 = Smooth(int(x) + 1, int(y) + 1);

	// interpolates
	float i1 = Interpolate(v1, v2, fractional_X);
	float i2 = Interpolate(v3, v4, fractional_X);

	float final = Interpolate(i1, i2, fractional_Y);

	return final;
}

float TotalNoisePerPoint(int x, int y)
{
	int octaves = 2;
	float zoom = 30.0f;
	float persistance = 0.9f;

	float total = 0.0f;

	for (int i = 0; i < octaves - 1; i++) {

		float frequency = pow(2, i) / zoom;
		float amplitude = pow(persistance, i);

		total += Noise(x * frequency, y * frequency) * amplitude;
	}

	return total;
}

void main() {
	vec2 avrageTessCord = (gs_in[0].texCord + gs_in[1].texCord + gs_in[2].texCord) / 3.0f;
	vec4 image = texture(gSampler, avrageTessCord.ts);
	vec4 avragePos = (gs_in[0].position + gs_in[1].position + gs_in[2].position) / 3.0f;
	vec3 avrageWorldPos = (gs_in[0].worldPosition + gs_in[1].worldPosition + gs_in[2].worldPosition) / 3.0f;
	int semiRandom = int(avrageWorldPos.x * 154.4f + avrageWorldPos.y * 864.3f + avrageWorldPos.z * 539.2f) % 2; // Add a chance to not create the grass to get rid of obvious patterns

	// Create the grass geomatry
	if(image.r < 0.5f && semiRandom != 0)
	{
		float xOffset = TotalNoisePerPoint(
			int((currentTime * 100.0f) + avrageWorldPos.x + 2000),
			int((currentTime * 100.0f) + avrageWorldPos.z + 2000));

		isGrass = 1.0f;
		outColor = gs_in[0].color;
		gl_Position= avragePos + mvp * vec4(-0.5f, 0.0f, 0.0f, 0.0f);
		FragPosLightSpace = gs_in[0].FragPosLightSpace;
		textureCord = vec2(0.0f, 1.0f);
		EmitVertex();
		outColor = gs_in[1].color;
		gl_Position= avragePos + mvp * vec4(0.5f, 0.0f, 0.0f, 0.0f);
		FragPosLightSpace = gs_in[1].FragPosLightSpace;
		textureCord = vec2(1.0f, 1.0f);
		EmitVertex();
		outColor = gs_in[2].color;
		gl_Position= avragePos + mvp * vec4(0.5f + xOffset, 1.0f, 0.0f, 0.0f);
		FragPosLightSpace = gs_in[2].FragPosLightSpace;
		textureCord = vec2(1.0f, 0.0f);
		EmitVertex();
		EndPrimitive();

		outColor = gs_in[0].color;
		gl_Position = avragePos + mvp * vec4(0.5f + xOffset, 1.0f, 0.0f, 0.0f);
		FragPosLightSpace = gs_in[0].FragPosLightSpace;
		textureCord = vec2(1.0f, 0.0f);
		EmitVertex();
		outColor = gs_in[1].color;
		gl_Position = avragePos + mvp * vec4(-0.5f + xOffset, 1.0f, 0.0f, 0.0f);
		FragPosLightSpace = gs_in[1].FragPosLightSpace;
		textureCord = vec2(0.0f, 0.0f);
		EmitVertex();
		outColor = gs_in[2].color;
		gl_Position = avragePos + mvp * vec4(-0.5f, 0.0f, 0.0f, 0.0f);
		FragPosLightSpace = gs_in[2].FragPosLightSpace;
		textureCord = vec2(0.0f, 1.0f);
		EmitVertex();
		EndPrimitive();

		isGrass = 1.0f;
		outColor = gs_in[0].color;
		gl_Position= avragePos + mvp * vec4(0.0f, 0.0f, -0.5f, 0.0f);
		FragPosLightSpace = gs_in[0].FragPosLightSpace;
		textureCord = vec2(0.0f, 1.0f);
		EmitVertex();
		outColor = gs_in[1].color;
		gl_Position= avragePos + mvp * vec4(0.0f, 0.0f, 0.5f, 0.0f);
		FragPosLightSpace = gs_in[1].FragPosLightSpace;
		textureCord = vec2(1.0f, 1.0f);
		EmitVertex();
		outColor = gs_in[2].color;
		gl_Position= avragePos + mvp * vec4(xOffset, 1.0f, 0.5f, 0.0f);
		FragPosLightSpace = gs_in[2].FragPosLightSpace;
		textureCord = vec2(1.0f, 0.0f);
		EmitVertex();
		EndPrimitive();

		outColor = gs_in[0].color;
		gl_Position = avragePos + mvp * vec4(xOffset, 1.0f, 0.5f, 0.0f);
		FragPosLightSpace = gs_in[0].FragPosLightSpace;
		textureCord = vec2(1.0f, 0.0f);
		EmitVertex();
		outColor = gs_in[1].color;
		gl_Position = avragePos + mvp * vec4(xOffset, 1.0f, -0.5f , 0.0f);
		FragPosLightSpace = gs_in[1].FragPosLightSpace;
		textureCord = vec2(0.0f, 0.0f);
		EmitVertex();
		outColor = gs_in[2].color;
		gl_Position = avragePos + mvp * vec4(0.0f, 0.0f, -0.5f , 0.0f);
		FragPosLightSpace = gs_in[2].FragPosLightSpace;
		textureCord = vec2(0.0f, 1.0f);
		EmitVertex();
		EndPrimitive();
	}

	// Create the ground geomatry
	isGrass = 0.0f;
	outColor = gs_in[0].color;
	gl_Position = gs_in[0].position;
	FragPosLightSpace = gs_in[0].FragPosLightSpace;
	textureCord = gs_in[0].texCord;
	EmitVertex();

	outColor = gs_in[1].color;
	gl_Position = gs_in[1].position;
	FragPosLightSpace = gs_in[1].FragPosLightSpace;
	textureCord = gs_in[1].texCord;
	EmitVertex();

	outColor = gs_in[2].color;
	gl_Position = gs_in[2].position;
	FragPosLightSpace = gs_in[2].FragPosLightSpace;
	textureCord = gs_in[2].texCord;
	EmitVertex();
	EndPrimitive();
}