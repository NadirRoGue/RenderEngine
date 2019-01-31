#version 430 core

//Color de salida
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outEmission;
layout (location=2) out vec4 outGodRays;

in vec2 texCoord;

uniform int frameIter;
uniform mat4 oldProjView;

uniform vec3 realLightColor;

// Temporal reprojection texture buffers
uniform sampler2D repro1;
uniform sampler2D repro2;


uniform sampler2D vel1;
uniform sampler2D vel2;

uniform vec2 texelSize;



#define maskSize 8u
uniform vec2 bigaffectedTexels[maskSize] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(1.0,0.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));

vec4 reprojectedSample(in vec2 screenUV, in vec2 uv, in sampler2D colorSampler, in sampler2D velocitySampler)
{
	vec4 velocity = vec4(texture(velocitySampler, uv).xyz, 1.0);
	
	if(length(velocity.xyz) == 0.0)
		return vec4(0);

	vec4 oldProjection = oldProjView * velocity;
	oldProjection = oldProjection / oldProjection.w;
	vec2 reprojectedUV = (oldProjection.xy * vec2(0.5) + vec2(0.5));
	
	float x = reprojectedUV.x;
	float y = reprojectedUV.y;

	reprojectedUV = x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0? screenUV : reprojectedUV;

	vec2 diff = reprojectedUV - screenUV;
	vec2 finalUV = x > 0.5? (reprojectedUV - vec2(0.5, 0.0)) / vec2(0.5, 1.0) : reprojectedUV / vec2(0.5, 1.0);//uv + diff;
	float u = finalUV.x;
	float v = finalUV.y;
	//return vec4(finalUV, 0, 1);
	return texture(colorSampler, finalUV);
}

vec4 sampleTexture(in vec2 screenUV, in vec2 uv, in sampler2D colorSampler, in sampler2D velocitySampler, in bool reproject)
{
	return reproject? reprojectedSample(screenUV, uv, colorSampler, velocitySampler) : texture(colorSampler, uv);
}

vec4 getCloudInfo(vec2 modUV)
{
	// Coordinates we are treating
	int x = int(gl_FragCoord.x + modUV.x);
	float y = gl_FragCoord.y - 0.5 + modUV.y;

	// normalized coordinates
	float u = (float(x) * texelSize.x);
	float v = y * texelSize.y;

	vec2 realUV = vec2(u, v);

	//return vec4(realUV, 0, 1);

	//vec2 reproUV = vec2(fract(u / 0.5), v);
	//bool reproLeft = frameIter == 1;

#define INTERLEAVED
#ifdef INTERLEAVED
	int coordIter = x % 2;
	return coordIter == 0? texture(repro1, realUV) : texture(repro2, realUV);//texture(repro1, realUV) :texture(repro2, realUV);
#else
	return u <= 0.5? sampleTexture(realUV, realUV / vec2(0.5, 1.0), repro1, vel1, frameIter == 1) : sampleTexture(realUV, (realUV - vec2(0.5, 0.0)) / vec2(0.5, 1.0), repro2, vel2, frameIter == 0);
	//return u < 0.5? sampleTexture(realUV, reproUV, repro1, vel1, reproLeft) : (u > 0.5? sampleTexture(realUV, reproUV, repro2, vel2, !reproLeft) : (sampleTexture(realUV, vec2(0.5, v), repro1, vel1, reproLeft) + sampleTexture(realUV, vec2(0.0, v), repro2, vel2, !reproLeft)) / 2.0);
#endif
}

void main()
{
	vec4 color = getCloudInfo(vec2(0));

	// Check neighbours
	//vec4 checkColor = vec4(0);
	//for (uint i = 0u; i < maskSize; i++)
	//{
	//	checkColor += getCloudInfo(bigaffectedTexels[i]);
	//}

	//checkColor /= float(maskSize);
	//vec4 diff = color - checkColor;
	//color = length(diff) > length(checkColor) * 0.01? checkColor : color;

	// Color info
	outColor = color;
	// Emission info (bloom post process)
	outEmission = vec4(0,0,0, color.a);
	// Increase color intensity on edges to produce a more beautiful god ray's effect
	vec3 gr = color.rgb * clamp(1.0 - color.a, 0, 1) * 1.0;
	outGodRays = vec4(gr, color.a);
	gl_FragDepth = 0.999;
}