#version 410 core

layout (location=0) out vec4 color;
layout (location=0) in vec3 inCubeUV;


uniform samplerCube skybox;
uniform vec3 lightDir;
uniform float radius = 0.9;
uniform vec3 lightColor;


void main()
{
	vec3 uv = inCubeUV;
	float realY = uv.y;
	uv.y = -abs(realY);

	vec3 camToSphere = normalize(uv * radius);
	float dotValue = dot(camToSphere, lightDir);

	float treeshold = 1.0 - 0.999;
	float value = 1.0 - dotValue;
	float alpha = value / treeshold;

	float treeshold2 = 1.0 - 0.9985;
	float alpha2 = value / treeshold2;

	vec3 up = vec3(0,1,0);
	float colorFactor = dot(up, -lightDir) + 0.25;
	vec3 centerColor = lightColor;
	centerColor.y *= colorFactor;
	centerColor.z *= colorFactor;
	vec3 edgeColor = centerColor;
	edgeColor.y *= 0.85;
	edgeColor.z *= 0.85;

	vec3 cubeMapColor = texture(skybox, uv).rgb;
	vec3 skyColor = dotValue > 0.999 && realY < 0.0 ? mix(centerColor, edgeColor, alpha*alpha*alpha*alpha) : dotValue > 0.9985 && realY < 0.0 ? mix(edgeColor, cubeMapColor, alpha2*alpha2) : cubeMapColor * colorFactor;

	color = vec4(skyColor, 1.0);
}