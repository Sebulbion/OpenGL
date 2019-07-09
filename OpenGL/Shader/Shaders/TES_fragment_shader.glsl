#version 430 core
in vec4 outColor;
in vec2 textureCord;
in float isGrass;
in vec4 FragPosLightSpace;
//in vec3 normal;
uniform sampler2D gSampler;
uniform sampler2D gGrassSampler;
uniform sampler2D shadowMap;
//uniform vec3 gCameraFront;
out vec4 color;
vec3 lightDir = vec3(1.0f, -1.0f, 1.0f);

// A 3 value interpolation to go from brown to gray to white
vec4 InterpolateColor(float weight)
{
	if (weight < 0.66f)
	{
		weight *= 1.5151f;
		return mix(vec4(0.6f,0.3f,0.15f, 1.0f), vec4(0.6f,0.6f,0.6f, 1.0f), weight);
	}
	else
	{
		weight = (weight - 0.5f) * 2.0f;	// If I wanted the interpolation to be smooth it should be (weight - 0.66f) * 3.0f
		return mix(vec4(0.6f,0.6f,0.6f, 1.0f), vec4(1.0f,1.0f,1.0f, 1.0f), weight);
	}
}
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
void main(){
	// Draw differently depending on if it's grass or not
	if (isGrass != 0.0f)
	{
		vec4 image = texture(gGrassSampler, textureCord.st);
		if (image.a < 0.5f)
			discard;
		float shadow = ShadowCalculation();
		color = image * outColor * shadow;
	}
	else
	{
		float shadow = ShadowCalculation();
		//vec4 testImage = texture(shadowMap, textureCord.xy);
		vec4 image = texture(gSampler, textureCord.ts);
		image = InterpolateColor(image.r);
		//color = testImage;
		color = outColor * image * shadow;
	}
}