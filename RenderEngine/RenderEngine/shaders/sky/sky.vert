#version 410 core

layout (location=0) in vec3 inPos;

uniform mat4 proj;

layout (location=0) out vec3 outCubeUV;

void main()
{
	outCubeUV = inPos * -1;
	vec4 pos = proj * vec4(inPos, 1.0);
	gl_Position = pos.xyww;
}