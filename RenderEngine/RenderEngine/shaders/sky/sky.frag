#version 410 core

layout (location=0) out vec4 color;

layout (location=0) in vec3 inCubeUV;

uniform samplerCube skybox;

void main()
{
	vec3 uv = inCubeUV;
	uv.y = -abs(uv.y);
	vec3 skyColor = texture(skybox, uv).rgb;

	color = vec4(skyColor, 1.0);
}