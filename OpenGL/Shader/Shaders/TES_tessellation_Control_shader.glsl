#version 430 core

//size of output patch & no. of times the TCS will be executed 
layout (vertices = 3) out; 

uniform vec4 cameraPos;
uniform int levelOfDetail;

in vec3 tcsColor[];
out vec3 tesColor[];
in vec2 tcsTexCord[];
out vec2 tesTexCord[];

float GetAvrageDistance(float Distance0, float Distance1)
{
	float AvgDistance = (Distance0 + Distance1) / 2.0;
	return AvgDistance;
}

void main()
{
	float cameraDist;
	float maxDist = 150.0f;
	float minDist = 80.0f;
	float maxInnerLevel = levelOfDetail;
	float minInnerLevel = levelOfDetail/2.0f;
	float percentage;
	float innerLevel;
	float outerLevel = 0.0f;

	if (gl_InvocationID == 0)
	{
		// Calculate the distance from the camera to the three control points
		float EyeToVertexDistance0 = distance(cameraPos, gl_in[0].gl_Position);
		float EyeToVertexDistance1 = distance(cameraPos, gl_in[1].gl_Position);
		float EyeToVertexDistance2 = distance(cameraPos, gl_in[2].gl_Position);

		// Calculate the tessellation levels
		cameraDist = abs(length(GetAvrageDistance(EyeToVertexDistance1, EyeToVertexDistance2)));
		percentage = clamp(cameraDist, minDist, maxDist) / maxDist;
		innerLevel = mix(maxInnerLevel, minInnerLevel, percentage);
		gl_TessLevelOuter[0] = innerLevel;

		cameraDist = abs(length(GetAvrageDistance(EyeToVertexDistance2, EyeToVertexDistance0)));
		percentage = clamp(cameraDist, minDist, maxDist) / maxDist;
		innerLevel = mix(maxInnerLevel, minInnerLevel, percentage);
		gl_TessLevelOuter[1] = innerLevel;

		cameraDist = abs(length(GetAvrageDistance(EyeToVertexDistance0, EyeToVertexDistance1)));
		percentage = clamp(cameraDist, minDist, maxDist) / maxDist;
		innerLevel = mix(maxInnerLevel, minInnerLevel, percentage);
		gl_TessLevelOuter[2] = innerLevel;

		// Make the inner level always odd
		outerLevel = (floor(gl_TessLevelOuter[2] / 2.0f) + 1.0f) * 2.0f;
		gl_TessLevelInner[0] = outerLevel;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tesColor[gl_InvocationID] = tcsColor[gl_InvocationID];
	tesTexCord[gl_InvocationID] = tcsTexCord[gl_InvocationID];
}