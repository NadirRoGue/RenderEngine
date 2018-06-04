#version 430 core

#define MASK_MAX_SIZE 64u

//Color de salida
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outEmission;
layout (location=2) out vec4 outGodRays;

in vec2 texCoord;

//Textura
uniform sampler2D gColor;
uniform sampler2D gEmission;
uniform sampler2D gGodRays;

uniform uint maskSize = 25u;
uniform vec2 texelSize;

#define maskFactor 1.0/65.0

uniform vec2 bigaffectedTexels[25u] = vec2[](
	vec2(-2.0,2.0), vec2(-1.0,2.0), vec2(0.0,2.0), vec2(1.0,2.0), vec2(2.0,2.0),
	vec2(-2.0,1.0), vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0), vec2(2.0,1.0),
	vec2(-2.0,0.0), vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0), vec2(2.0,0.0),
	vec2(-2.0,-1.0), vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0), vec2(2.0,-1.0),
	vec2(-2.0,-2.0), vec2(-1.0,-2.0), vec2(0.0,-2.0), vec2(1.0,-2.0), vec2(2.0,-2.0));

uniform float bigkernel[25u] = float[](
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	3.0*maskFactor, 4.0*maskFactor, 5.0*maskFactor,4.0*maskFactor, 3.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor);

void main()
{
	vec4 color = vec4 (0.0);

	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + texelSize * bigaffectedTexels[i];
		color += texture(gColor, iidx,0.0) * bigkernel[i];
	}

	outColor = color;
	outEmission = vec4(0,0,0, color.a);
	vec3 gr = color.rgb * (1.0 - color.a);
	outGodRays = vec4(gr, color.a);
	gl_FragDepth = 0.999;
}