#version 430 core

layout (location = 0) out vec4 outColor;

in vec2 texCoord;

uniform mat4 projMat;

uniform vec3 lightDirection;

uniform sampler2D postProcessing_0;	// color

uniform sampler2D posBuffer;
uniform sampler2D depthBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D specularBuffer;

vec3 raymarch(vec3 position, vec3 direction, float step)
{
	vec3 prevRaySample, raySample;
	// Screen space ray march
	for (int rayStepIdx = 0; rayStepIdx < 16; rayStepIdx++)
	{
		// Compare current ray depth with scene's depth
		prevRaySample = raySample;
		raySample = (rayStepIdx * step) * direction + position;
		float zBufferVal = texture(depthBuffer, raySample.xy).x;
				
		// If we are in a position with a higher depth,
		// it means there was something with less depth,
		// thus we have gone throught it, and should be reflected
		if (raySample.z > zBufferVal )
		{
			vec3 minRaySample = prevRaySample;
			vec3 maxRaySample = raySample;
			vec3 midRaySample;
			// Binary search
			for (int i = 0; i < 6; i++)
			{
				midRaySample = mix(minRaySample, maxRaySample, 0.5);
				zBufferVal = texture(depthBuffer, midRaySample.xy).x;

				if (midRaySample.z > zBufferVal)
					maxRaySample = midRaySample;
				else
					minRaySample = midRaySample;
			}

			return texture(postProcessing_0, midRaySample.xy).rgb;
		}
	}

	return texture(postProcessing_0, texCoord).rgb;
}

vec3 computeReflectionColor(float depth, vec3 pos, vec3 N)
{
	// screen space pos as depth
	vec3 ssPos = vec3(texCoord, depth);

	// Reflect view dir
	vec3 camReflect = reflect(-pos, N);

	// Screen space position from reflected view dir
	vec3 pointAlongRefl = camReflect * 10.0 + pos;
	vec4 projPointAlong = projMat * vec4(pointAlongRefl, 1);
	projPointAlong /= projPointAlong.w;
	projPointAlong.xy = projPointAlong.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);

	// Screen space reflection dir
	vec3 ssreflectdir = normalize(projPointAlong.xyz - ssPos);
	
	float viewFactor = clamp(dot(normalize(-pos), N),0,1);

	return raymarch(ssPos, ssreflectdir, 0.02);
}

void main()
{
	vec3 pos = texture(posBuffer, texCoord).xyz;
	vec3 N = texture(normalBuffer, texCoord).xyz;
	float depth = texture(depthBuffer, texCoord).x;
	float reflection = texture(specularBuffer, texCoord).w;

	// Compute relfection only for specular surfaces
	if(reflection > 0.0)
	{
		outColor = vec4(computeReflectionColor(depth, pos, N), 1.0) * 0.5 + texture(postProcessing_0, texCoord) * 0.5;
	}
	else
	{
		outColor = texture(postProcessing_0, texCoord);
	}
}