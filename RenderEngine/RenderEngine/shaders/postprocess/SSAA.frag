#version 430 core

#define MASK_MAX_SIZE 64u

layout (location=0) out vec4 outColor;

layout (location=0) in vec2 texCoord;

// Input color
uniform sampler2D postProcessing_0;

// 1.0 / screenResolution
uniform vec2 texelSize;

// Masks are fixed, there is no need to uplaod them from the shader
// Declared as uniform and initialized in the shader
uniform const vec2 texIdx[9u] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));

uniform const float Gx[9u] = float[](
	float (1.0), float (0.0), float (-1.0),
	float (2.0), float (0.0), float (-2.0),
	float (1.0), float (0.0), float (-1.0)
); 

uniform const float Gy[9u] = float[](
	float (1.0), float (2.0), float (1.0),
	float (0.0), float (0.0), float (0.0),
	float (-1.0), float (-2.0), float (-1.0)
);

const vec2 bigTexIdx[25u] = vec2[](
	vec2(-2.0,2.0), vec2(-1.0,2.0), vec2(0.0,2.0), vec2(1.0,2.0), vec2(2.0,2.0),
	vec2(-2.0,1.0), vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0), vec2(2.0,1.0),
	vec2(-2.0,0.0), vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0), vec2(2.0,0.0),
	vec2(-2.0,-1.0), vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0), vec2(2.0,-1.0),
	vec2(-2.0,-2.0), vec2(-1.0,-2.0), vec2(0.0,-2.0), vec2(1.0,-2.0), vec2(2.0,-2.0));

const float maskFactor = float (1.0/65.0);

const float AAMask[25u] = float[](
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	3.0*maskFactor, 4.0*maskFactor, 5.0*maskFactor,4.0*maskFactor, 3.0*maskFactor,
	2.0*maskFactor, 3.0*maskFactor, 4.0*maskFactor,3.0*maskFactor, 2.0*maskFactor,
	1.0*maskFactor, 2.0*maskFactor, 3.0*maskFactor,2.0*maskFactor, 1.0*maskFactor);


void main()
{
	vec3 vertical = vec3(0);
	vec3 horizontal = vec3(0);

	for (uint i = 0u; i < 9u; i++)
	{
		vec2 iidx = texCoord + texelSize * texIdx[i];
		vertical += texture(postProcessing_0, iidx,0.0).xyz * Gy[i];
		horizontal += texture(postProcessing_0, iidx,0.0).xyz * Gx[i];
	}

	vec3 temp = sqrt((vertical * vertical) + (horizontal * horizontal));
	float c = length(temp);

	float applyMask = c > 0.85? 1.0 : 0.0;

	vec3 color = vec3 (0.0);
	for (uint i = 0u; i < 25u; i++)
	{
		vec2 iidx = texCoord + texelSize * bigTexIdx[i] * applyMask;
		color += texture(postProcessing_0, iidx,0.0).rgb * AAMask[i];
	}

	outColor = vec4(color.rgb, 1);
}