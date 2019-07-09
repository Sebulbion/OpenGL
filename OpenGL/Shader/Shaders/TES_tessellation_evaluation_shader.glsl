#version 430 core

layout (triangles, equal_spacing, ccw) in;

uniform mat4 mvp;
uniform mat4 lightMvp;

out VS_GS_VERTEX{
	out vec4 position;
	out vec4 FragPosLightSpace;
	out vec4 color;
	out vec2 texCord;
	out vec3 worldPosition;
} tse_out;

in vec3 tesColor[];
in vec2 tesTexCord[];

uniform sampler2D gSampler;
uniform float gMaxHeight;

void main()
{
	tse_out.texCord = (
		gl_TessCoord.x * tesTexCord[0] +
		gl_TessCoord.y * tesTexCord[1] +
		gl_TessCoord.z * tesTexCord[2]);

	vec4 image = texture(gSampler, tse_out.texCord.ts);
	float tesHeight = image.r * gMaxHeight;	

	vec4 position0 = gl_TessCoord.x * vec4(gl_in[0].gl_Position.x, tesHeight, gl_in[0].gl_Position.z, gl_in[0].gl_Position.w);
	vec4 position1 = gl_TessCoord.y * vec4(gl_in[1].gl_Position.x, tesHeight, gl_in[1].gl_Position.z, gl_in[1].gl_Position.w);
	vec4 position2 = gl_TessCoord.z * vec4(gl_in[2].gl_Position.x, tesHeight, gl_in[2].gl_Position.z, gl_in[2].gl_Position.w);

	// Apply level of detail on the height
	vec4 worldPos = (
		position0 +
		position1 +
		position2);

	tse_out.worldPosition = worldPos.xyz;
	gl_Position = mvp * worldPos;
	

	vec3 interpolatedColor = (
		gl_TessCoord.x * tesColor[0] +
		gl_TessCoord.y * tesColor[1] +
		gl_TessCoord.z * tesColor[2]);

	tse_out.color = vec4(interpolatedColor, 1.0f);
	tse_out.position= gl_Position;
	tse_out.FragPosLightSpace = lightMvp * worldPos;
}