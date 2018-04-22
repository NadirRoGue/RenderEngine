#version 410 core

// INPUT
layout (location=0) in vec3 inPos;
layout (location=1) in vec2 inUV;

// OUTPUT
layout (location=0) out vec2 outUV;

uniform ivec2 gridPos;

void main()
{
	gl_Position = vec4(inPos, 1.0);
	outUV = abs(inUV + vec2(float(gridPos.x), float(gridPos.y)));
}