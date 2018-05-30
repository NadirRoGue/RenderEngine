#version 410 core

// INPUT
layout (location=0) in vec3 inPos;
layout (location=1) in vec2 inUV;

// OUTPUT
layout (location=0) out vec2 outUV;
layout (location=1) out vec3 outPos;
layout (location=2) out vec4 outShadowMapPos;
layout (location=3) out vec4 outShadowMapPos1;

uniform ivec2 gridPos;

uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 lightDepthMat;
uniform mat4 lightDepthMat1;

void main()
{
#ifndef SHADOW_MAP
	gl_Position = modelViewProj * vec4(inPos, 1.0);
	outPos = (modelView * vec4(inPos, 1.0)).xyz;
	outUV = abs(inUV + vec2(float(gridPos.x), float(gridPos.y)));
	outShadowMapPos = lightDepthMat * vec4(inPos, 1.0);
	outShadowMapPos1 = lightDepthMat1 * vec4(inPos, 1.0);
#else
	gl_Position = lightDepthMat * vec4(inPos, 1);
#endif
}