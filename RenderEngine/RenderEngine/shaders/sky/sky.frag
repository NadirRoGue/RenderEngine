#version 410 core

layout (location=0) out vec4 color;
layout (location=1) out vec4 emission;
layout (location=2) out vec4 godRayInfo;

layout (location=0) in vec3 inCubeUV;

uniform vec3 lightDir;
uniform float radius = 0.9;
uniform vec3 lightColor;
uniform float colorFactor;

uniform vec3 zenitColor;
uniform vec3 horizonColor;

void main()
{
	// Position as radius within a sphere
	vec3 uv = inCubeUV;
	float realY = uv.y;
	uv.y = -abs(realY);
	vec3 camToSphere = normalize(uv * radius);

	// Only these fragments whose sphere position angle is less than a threshold will be sun
	float dotValue = dot(camToSphere, lightDir);

	float alphaSky = -camToSphere.y * 0.6;
	vec3 cubeMapColor = mix(horizonColor, zenitColor, alphaSky);
	
	color = realY < 0 && dotValue > 0.999? vec4(lightColor, 1.0) : vec4(cubeMapColor * colorFactor, 1.0);
	emission = realY < 0 && dotValue > 0.999? vec4(lightColor, 1) : vec4(0,0,0,1);
	godRayInfo = realY < 0 && dotValue > 0.999? vec4(lightColor, 1.0) : vec4(0);
}