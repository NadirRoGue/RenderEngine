#version 430 core

layout (location=0) in vec3 inPos;

layout (location=0) out vec3 outPos;

uniform mat4 lightProjMat;

void main()
{
	outPos = inPos;
	// Project pos from light point of view
	gl_Position = lightProjMat * vec4(inPos, 1);
}