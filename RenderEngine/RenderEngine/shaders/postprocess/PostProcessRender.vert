#version 430 core

layout (location=0) in vec3 inPos;	
layout (location=1) in vec2 inTexCoord;

layout (location=0) out vec2 texCoord;
layout (location=1) out vec3 planePos;

void main()
{
	texCoord = inTexCoord;//vec2(0.5) + inPos.xy * 0.5 ;
	planePos = inPos;
	gl_Position = vec4(inPos,1);
}
