#version 330 core

layout (location=0) in vec3 inPos;	
layout (location=1) in vec2 inTexCoord;

out vec2 texCoord;

void main()
{
	texCoord = inTexCoord;//vec2(0.5) + inPos.xy * 0.5 ;
	
	gl_Position = vec4(inPos,1);
}
