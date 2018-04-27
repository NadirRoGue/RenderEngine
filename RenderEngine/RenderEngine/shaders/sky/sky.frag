#version 410 core

layout (location=0) out vec4 color;

layout (location=0) in vec3 inCubeUV;


uniform samplerCube skybox;

uniform vec3 lightDir;

uniform float radius = 0.9;

uniform vec3 centerColor = vec3(1,1,0.9);
uniform vec3 edgeColor = vec3(1, 0.9, 0.8);

void main()
{
	vec3 uv = inCubeUV;
	uv.y = -abs(uv.y);

	vec3 camToSphere = normalize(uv * radius);
	float dotValue = dot(camToSphere, lightDir);

	float treeshold = 1.0 - 0.999;
	float value = 1.0 - dotValue;
	float alpha = value / treeshold;

	float treeshold2 = 1.0 - 0.9985;
	float alpha2 = value / treeshold2;

	vec3 cubeMapColor = texture(skybox, uv).rgb;
	vec3 skyColor = dotValue > 0.999 ? mix(centerColor, edgeColor, alpha*alpha*alpha*alpha) : dotValue > 0.9985? mix(edgeColor, cubeMapColor, alpha2*alpha2) : cubeMapColor;

	color = vec4(skyColor, 1.0);
}