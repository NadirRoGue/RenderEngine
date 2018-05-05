#version 430 core

layout (location=0) in vec3 inPos;	
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inNormal;
layout (location=3) in vec3 inEmission;

//uniform mat4 modelViewProj;
//uniform mat4 modelView;
//uniform mat4 normal;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outEmission;


void main()
{
	outPos = inPos; //(modelView * vec4(inPos, 1)).xyz;
	outColor = inColor;
	outEmission = inEmission;
	outNormal = inNormal; //(normal * vec4(inNormal, 0)).xyz;
	
	gl_Position = vec4(inPos, 1);//modelViewProj * vec4(inPos, 1);
}
