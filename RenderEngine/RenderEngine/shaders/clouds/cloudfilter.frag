#version 430 core

#define MASK_MAX_SIZE 64u

//Color de salida
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outEmission;
layout (location=2) out vec4 outGodRays;

in vec2 texCoord;

uniform vec3 realLightColor;

//Textures
uniform sampler2D repro1;
uniform sampler2D repro2;
//uniform sampler2D repro3;
//uniform sampler2D repro4;

uniform vec2 texelSize;

#define BIG_KERNEL

#ifdef BIG_KERNEL
#define maskSize 25u
#define maskFactor 1.0/65.0
uniform vec2 bigaffectedTexels[maskSize] = vec2[](
	vec2(-2.0,2.0), vec2(-1.0,2.0), vec2(0.0,2.0), vec2(1.0,2.0), vec2(2.0,2.0),
	vec2(-2.0,1.0), vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0), vec2(2.0,1.0),
	vec2(-2.0,0.0), vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0), vec2(2.0,0.0),
	vec2(-2.0,-1.0), vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0), vec2(2.0,-1.0),
	vec2(-2.0,-2.0), vec2(-1.0,-2.0), vec2(0.0,-2.0), vec2(1.0,-2.0), vec2(2.0,-2.0));

uniform float bigkernel[maskSize] = float[](
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	3.0*maskFactor, 4.0*maskFactor, 5.0*maskFactor,4.0*maskFactor, 3.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor);

#else
#define maskSize 9u
#define maskFactor 1.0/14.0
uniform vec2 bigaffectedTexels[maskSize] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));

uniform float bigkernel[maskSize] = float[](
	1.0*maskFactor, 2.0*maskFactor, 1.0*maskFactor,
	2.0*maskFactor, 2.0*maskFactor, 2.0*maskFactor,
	1.0*maskFactor, 2.0*maskFactor, 1.0*maskFactor); 

#endif

vec4 getCloudInfo(vec2 modUV)
{
	int x = int(gl_FragCoord.x + modUV.x);
	float y = gl_FragCoord.y + modUV.y;

	float u = (float(x) * texelSize.x);
	float v = y * texelSize.y;

	vec2 realUV = vec2(u, v);

	int coordIter = x % 2;
	return coordIter == 0? texture(repro1, realUV) :texture(repro2, realUV);
	//return coordIter == 0? texture(repro1, realUV) : coordIter == 1? texture(repro2, realUV) : coordIter == 2? texture(repro3, realUV) : texture(repro4, realUV);
}

void main()
{
	vec4 color = vec4 (0.0);

	for (uint i = 0u; i < maskSize; i++)
	{
		color += getCloudInfo(bigaffectedTexels[i]) * bigkernel[i];
	}

	outColor = color;
	outEmission = vec4(0,0,0, color.a);
	vec3 gr = (realLightColor * 0.6) * (1.0 - color.a);
	outGodRays = vec4(gr, color.a);
	gl_FragDepth = 0.999;
}