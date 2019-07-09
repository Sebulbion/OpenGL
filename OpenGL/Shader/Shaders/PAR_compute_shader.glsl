#version 440 core

#define WORK_GROUP_SIZE 128

layout(local_size_x = WORK_GROUP_SIZE, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer positionBuffer { vec4 position[]; };
layout(std430, binding = 1) buffer initPositionBuffer { vec4 initPosition[]; };
layout(std430, binding = 2) buffer velocityBuffer { vec4 velocity[]; };
layout(std430, binding = 3) buffer initVelocityBuffer { vec4 initVelocity[]; };


uniform float currentTime;
void main(void)
{
	uint i = gl_GlobalInvocationID.x;

	vec3 gravity = vec3(sin(currentTime * i) / 100.0f, -0.03f * .0167f, cos(currentTime * i) / 100.0f);
	velocity[i].xyz += gravity;

	position[i].xyz += velocity[i].xyz;
	position[i].w -= 2.5f * .167f;

	if (position[i].w <= 0.0f || position[i].y < -90.0f) {
		position[i] = initPosition[i];
		position[i].w = initVelocity[i].w;

		velocity[i] = initVelocity[i];
	}
}