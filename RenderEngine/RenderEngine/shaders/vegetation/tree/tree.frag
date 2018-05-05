#version 430 core

layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 outSpecular;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outPos;

layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inNormal;
layout (location=3) in vec3 inEmission;

void main()
{
	outColor = vec4(inColor, 1);
	outNormal = vec4(normalize(inNormal), 0);
	outSpecular = vec4(0,0,0,0);
	outEmissive = vec4(inEmission,0);
	outPos = vec4(inPos, 1);
}