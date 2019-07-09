#version 420 core

in vec2 textureCord;

uniform sampler2D gSampler;
uniform sampler2D gDepthSampler;
uniform mat4 gInverseViewProjection;
uniform mat4 gPreviousViewProjection;
uniform int gMode;

void main(void)
{
	if (gMode == 0)													// Normal shader
	{
		gl_FragColor = texture(gSampler, textureCord.st);
	}
	else if (gMode == 1 || gMode == 2 || gMode == 3)				// Edge detection
	{
		const ivec2 offsets[9] = ivec2[](
			ivec2(-1, 1), // top-left
			ivec2(0, 1), // top-center
			ivec2(1, 1), // top-right
			ivec2(-1, 0),   // center-left
			ivec2(0, 0),   // center-center
			ivec2(1, 0),   // center-right
			ivec2(-1, -1), // bottom-left
			ivec2(0, -1), // bottom-center
			ivec2(1, -1)  // bottom-right    
			);

		float kernelX[9] = float[](
			1, 2, 1,
			0, 0, 0,
			-1, -2, -1
			);
		float kernelY[9] = float[](
			1, 0, -1,
			2, 0, -2,
			1, 0, -1
			);

		vec3 sampleTex[9];

		for (int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(textureOffset(gSampler, textureCord.st, offsets[i]));
		}

		vec3 col = vec3(0.0);
		vec3 row = vec3(0.0);
		vec3 final = vec3(0.0);
		for (int i = 0; i < 9; i++)
		{
			col += sampleTex[i] * kernelY[i];
			row += sampleTex[i] * kernelX[i];
		}

		final = sqrt(pow(col, 2.0f.xxx) + pow(row, 2.0f.xxx));

		if (gMode == 1)
		{
			gl_FragColor = vec4(final, 1.0f);
		}
		else if (gMode == 2 || gMode == 3)						// Edge detection with nicer colours
		{
			vec2 movecTexCoord = textureCord - 0.5f.xx;
			vec4 from = vec4(1.0f, 0.0f, 1.0f, 1.0f);
			vec4 to = vec4(0.0f, 1.0f, 1.0f, 1.0f);
			if (length(movecTexCoord) > 0.4f && gMode == 3)		// Frameing it
			{
				gl_FragColor = mix(from, to, textureCord.t);
				return;
			}

			vec4 image = texture(gSampler, textureCord.st);
			image = image + 0.3f.xxxx;
			image = pow(image, 10.0f.xxxx);
			image = clamp(image, 0.0f.xxxx, 1.0f.xxxx);
			//image = image.bbba;
			image = mix(from, to, (image.r + image.g + image.b) / 3.0f);
			image *= final.xyzx;

			gl_FragColor = image;
		}
	}
	// Magical code I only kind of understand
	else if (gMode == 4)
	{
		// Get the depth buffer value at this pixel.
		float zOverW = texture2D(gDepthSampler, textureCord).r;
		// H is the viewport position at this pixel in the range -1 to 1.
		vec4 H = vec4(textureCord.x * 2 - 1, (1 - textureCord.y) * 2 - 1,
			zOverW, 1);
		// Transform by the view-projection inverse.
		vec4 D = gInverseViewProjection * H;
		// Divide by w to get the world position.
		vec4 worldPos = D / D.w;

		// Current viewport position
		vec4 currentPos = H;
		// Use the world position, and transform by the previous view-
		// projection matrix.
		vec4 previousPos = gPreviousViewProjection * worldPos;
		// Convert to nonhomogeneous points [-1,1] by dividing by w.
		previousPos /= previousPos.w;
		// Clamp the values between 1 and -1
		currentPos = clamp(currentPos, -1.0f, 1.0f);
		previousPos = clamp(previousPos, -1.0f, 1.0f);
		// Use this frame's position and last frame's to compute the pixel
		// velocity.
		vec2 velocity = ((currentPos - previousPos) / 25.0f).xy;
		velocity.y = -velocity.y;

		// Get the initial color at this pixel.
		vec4 color = texture2D(gSampler, textureCord);
		vec2 varyingTextureCord = textureCord;
		varyingTextureCord += velocity;
		for (int i = 1; i < 20; ++i, varyingTextureCord += velocity)
		{
			// Sample the color buffer along the velocity vector.
			vec4 currentColor = texture2D(gSampler, varyingTextureCord);
			// Add the current color to our color sum.
			color += currentColor;
		}
		// Average all of the samples to get the final blur color.
		gl_FragColor = color / 20;
	}
	//else if (gMode == 4)										// Fog using the depth buffer
	//{
	//	vec4 image = texture(gSampler, textureCord.st);
	//	float f = 250.0f;	// Camera far plane (badly hardcoded)
	//	float n = 0.1f;		// Camera near plane (badly hardcoded)
	//	float z = (2 * n) / (f + n - texture2D(gDepthSampler, textureCord.st).x * (f - n));	// Depth linearized

	//	if (z < 0.25f)
	//		z = 0.0f;
	//	else
	//		z = (z - 0.25f) * 4.0f;
	//	vec4 fog = 1.0f.xxxx;

	//	gl_FragColor = texture2D(gDepthSampler, textureCord.st);
	//	//gl_FragColor = mix(image, fog, z);
	//}
}