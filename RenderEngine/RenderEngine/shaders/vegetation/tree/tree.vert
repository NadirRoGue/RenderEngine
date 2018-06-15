#version 430 core

layout (location=0) in vec3 inPos;	
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inNormal;
layout (location=3) in vec3 inEmission;
layout (location=4) in vec2 inTexCoord;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outEmission;
layout(location = 3) out vec2 outTexCoord;

void main()
{
	outColor = inColor;
	outEmission = inEmission;
	outNormal = inNormal;
	outTexCoord = inTexCoord;

	gl_Position = vec4(inPos, 1);
}
