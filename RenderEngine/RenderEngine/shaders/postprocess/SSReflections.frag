#version 430 core

layout (location = 0) out vec4 outColor;

in vec2 texCoord;

uniform mat4 projMat;

uniform sampler2D postProcessing_0;	// color

uniform sampler2D posBuffer;
uniform sampler2D depthBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D specularBuffer;

vec3 raymarch(vec3 position, vec3 direction)
{
	vec3 PrevRaySample, RaySample;
	for (int RayStepIdx = 0; RayStepIdx < 16; RayStepIdx++)
	{
		PrevRaySample = RaySample;
		RaySample = (RayStepIdx * 0.02) * direction + position;
		float ZBufferVal = texture(depthBuffer, RaySample.xy).x;
				
		if (RaySample.z > ZBufferVal )
		{
			vec3 MinRaySample = PrevRaySample;
			vec3 MaxRaySample = RaySample;
			vec3 MidRaySample;
			for (int i = 0; i < 6; i++)
			{
				MidRaySample = mix(MinRaySample, MaxRaySample, 0.5);
				float ZBufferVal = texture(depthBuffer, MidRaySample.xy).x;

				if (MidRaySample.z > ZBufferVal)
					MaxRaySample = MidRaySample;
				else
					MinRaySample = MidRaySample;
			}

			return texture(postProcessing_0, MidRaySample.xy).rgb;
		}
	}

	return texture(postProcessing_0, texCoord).rgb;
}

vec3 computeReflectionColor(float depth, vec3 pos, vec3 N)
{
	vec3 ssPos = vec3(texCoord, depth);

	vec3 camReflect = reflect(-pos, N);

	vec3 pointAlongRefl = camReflect * 10.0 + pos;
	vec4 projPointAlong = projMat * vec4(pointAlongRefl, 1);
	projPointAlong /= projPointAlong.w;
	projPointAlong.xy = projPointAlong.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);

	vec3 ssreflectdir = normalize(projPointAlong.xyz - ssPos);

	return raymarch(ssPos, ssreflectdir);
}

void main()
{
	vec3 pos = texture(posBuffer, texCoord).xyz;
	vec3 N = texture(normalBuffer, texCoord).xyz;
	float depth = texture(depthBuffer, texCoord).x;
	float reflection = texture(specularBuffer, texCoord).w;
	if(reflection > 0.0)
	{
		outColor = vec4(computeReflectionColor(depth, pos, N), 1.0);
	}
	else
	{
		outColor = texture(postProcessing_0, texCoord);
	}
}