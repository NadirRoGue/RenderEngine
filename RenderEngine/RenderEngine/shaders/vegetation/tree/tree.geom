#version 410 core

layout(triangles) in;
#ifdef WIRE_MODE
layout(line_strip, max_vertices=3) out;
#else
layout(triangle_strip, max_vertices=3) out;
#endif

#ifndef SHADOW_MAP
layout (location=0) in vec3 inColor[];
layout (location=1) in vec3 inNormal[];
layout (location=2) in vec3 inEmission[];
layout (location=3) in vec2 inTexCoord[];

layout (location=0) out vec3 outPos;
layout (location=1) out vec3 outColor;
layout (location=2) out vec3 outNormal;
layout (location=3) out vec3 outEmission;
layout (location=4) out vec3 lightDepth;
layout (location=5) out vec3 lightDepth1;
layout (location=6) out vec2 outTexCoord;

uniform mat4 normal;
uniform mat4 modelView;
uniform mat4 modelViewProj;
#endif

uniform float waterHeight;
uniform float maxHeight;

uniform mat4 lightDepthMat;
uniform mat4 lightDepthMat1;

uniform vec2 tileUV;

uniform float worldScale;

// ===============================================================================

uniform float amplitude;
uniform float frecuency;
uniform float scale;
uniform int octaves;

float Random2D(in vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float NoiseInterpolation(in vec2 i_coord, in float i_size)
{
	vec2 grid = i_coord * i_size;

	vec2 randomInput = floor(grid);
	vec2 weights = fract(grid);


	float p0 = Random2D(randomInput);
	float p1 = Random2D(randomInput + vec2(1.0, 0.0));
	float p2 = Random2D(randomInput + vec2(0.0, 1.0));
	float p3 = Random2D(randomInput + vec2(1.0, 1.0));

	weights = smoothstep(vec2(0.0, 0.0), vec2(1.0, 1.0), weights);

	return p0 +
		(p1 - p0) * (weights.x) +
		(p2 - p0) * (weights.y) * (1.0 - weights.x) +
		(p3 - p1) * (weights.y * weights.x);
}

float noiseHeight(in vec2 pos)
{

	float noiseValue = 0.0;

	float localAplitude = amplitude;
	float localFrecuency = frecuency;

	for (int index = 0; index < octaves; index++)
	{

		noiseValue += NoiseInterpolation(pos, scale * localFrecuency) * localAplitude;

		localAplitude /= 2.0;
		localFrecuency *= 2.0;
	}

	return noiseValue * noiseValue * noiseValue;
}

// ============================================================================

void main()
{
	
	float height = noiseHeight(tileUV);

	if(height > waterHeight && height < maxHeight)
	{
		vec4 displacement = vec4(0, height * 1.5 * worldScale, 0, 0);

		vec4 a = gl_in[0].gl_Position + displacement;
		vec4 b = gl_in[1].gl_Position + displacement;
		vec4 c = gl_in[2].gl_Position + displacement;

#ifndef SHADOW_MAP
		outTexCoord = inTexCoord[0];
		outColor = inColor[0];
		outEmission = inEmission[0];
		outNormal = (normal * vec4(inNormal[0], 0)).xyz;
		outPos = (modelView * a).xyz;
		lightDepth = (lightDepthMat * a).xyz;
		lightDepth1 = (lightDepthMat1 * a).xyz;
		gl_Position = modelViewProj * a;
		EmitVertex();

		outTexCoord = inTexCoord[1];
		outColor = inColor[1];
		outEmission = inEmission[1];
		outNormal = (normal * vec4(inNormal[1], 0)).xyz;
		outPos = (modelView * b).xyz;
		lightDepth = (lightDepthMat * b).xyz;
		lightDepth1 = (lightDepthMat1 * b).xyz;
		gl_Position = modelViewProj * b;
		EmitVertex();

		outTexCoord = inTexCoord[2];
		outColor = inColor[2];
		outEmission = inEmission[2];
		outNormal = (normal * vec4(inNormal[2], 0)).xyz;
		outPos = (modelView * c).xyz;
		lightDepth = (lightDepthMat * c).xyz;
		lightDepth1 = (lightDepthMat1 * c).xyz;
		gl_Position = modelViewProj * c;
		EmitVertex();
#else
		gl_Position = lightDepthMat * a;
		EmitVertex();

		gl_Position = lightDepthMat * b;
		EmitVertex();

		gl_Position = lightDepthMat * c;
		EmitVertex();
#endif
		EndPrimitive();
	}
}