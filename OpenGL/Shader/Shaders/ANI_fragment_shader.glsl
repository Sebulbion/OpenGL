#version 420 core

in vec2 TexCoord; 
in vec3 Normal; 
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 finalColor;

uniform vec3 objectColor;
uniform float specularStrength;
uniform float ambientStrength;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D texture_diffuse0;
uniform mat4 model;
uniform sampler2D shadowMap; 

float ShadowCalculation() 
{
	// To get NDC [-1, 1] from screenspace 
	vec3 ndcSpace = FragPosLightSpace.xyz / FragPosLightSpace.w;

	// Convert to Tex CoordSpace [0,1]
	vec3 texCoordSpace = 0.5f * ndcSpace + 0.5f;

	float bias = 0.000002f;
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
	vec3 lightDir = objectPos.xyz - lightPos;
	
	float intensity;
	float toonStrength;
	intensity = dot(lightDir,normalize(Normal));

	if (intensity > 0.95)
		toonStrength = 1.0f;
	else if (intensity > 0.5)
		toonStrength = 0.8f;
	else if (intensity > 0.25)
		toonStrength = 0.5f;
	else
		toonStrength = 0.2f;

	float shadow = ShadowCalculation();

	gl_FragColor = texture(texture_diffuse0, TexCoord) * vec4(objectColor, 1.0f) * toonStrength * shadow;
}