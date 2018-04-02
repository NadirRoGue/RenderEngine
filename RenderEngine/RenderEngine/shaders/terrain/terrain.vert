#version 450 core

layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inUV;

//layout (location=0) out vec3 outNormal;
//layout (location=1) out vec2 outUV;

out vec3 vNormal;
out vec2 vUV;

void main()
{
	gl_Position = vec4(inPos, 1.0);
	vNormal = inNormal;
	vUV = inUV;
}