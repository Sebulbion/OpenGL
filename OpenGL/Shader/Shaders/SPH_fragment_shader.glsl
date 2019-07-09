#version 420 core

in vec3 outColor;

uniform sampler2D gSampler;
uniform sampler2D shadowMap;
uniform mat4 model;
uniform vec3 lightPos;

in vec3 fragNormal;
in vec4 FragPosLightSpace;

float ShadowCalculation()
{
	// To get NDC [-1, 1] from screenspace 
	vec3 ndcSpace = FragPosLightSpace.xyz / FragPosLightSpace.w;

	// Convert to Tex CoordSpace [0,1]
	vec3 texCoordSpace = 0.5f * ndcSpace + 0.5f;

	float bias = 0.000004f;
	float currentDepth = texCoordSpace.z - bias;

	/*float closestDepth = texture(shadowMap, texCoordSpace.xy).r;
	float shadow = currentDepth < closestDepth ? 1.0 : 0.3;
	return shadow;*/

	float shadow;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(shadowMap, texCoordSpace.xy + vec2(x, y) * texelSize).x;
			shadow += currentDepth< pcfDepth ? 1.0 : 0.3;
		}
	}
	shadow /= 9.0;
	return shadow == 1.0 ? 1.0 : 0.3;
}

void main(void)
{
	vec4 objectPos = model * vec4(1.0f);
	vec3 lightDir = lightPos - objectPos.xyz;
	lightDir = normalize(lightDir);
	
	float intensity;
	float toonStrength;
	intensity = dot(lightDir, normalize(fragNormal));
	
	if (intensity > 0.70)
		toonStrength = 1.0f;
	else if (intensity > 0.25)
		toonStrength = 0.8f;
	else if (intensity > 0.0f)
		toonStrength = 0.5f;
	else
		toonStrength = 0.2f;

	float shadow = ShadowCalculation();

	gl_FragColor = vec4(outColor, 1.0f) * min(toonStrength, shadow);
}