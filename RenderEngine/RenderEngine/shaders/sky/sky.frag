#version 410 core

layout (location=0) out vec4 color;
layout (location=1) out vec4 emission;
layout (location=2) out vec4 godRayInfo;

layout (location=0) in vec3 inCubeUV;

uniform vec3 lightDir;
uniform float radius = 0.9;
uniform vec3 lightColor;

uniform vec3 zenitColor;
uniform vec3 horizonColor;

void main()
{
	vec3 uv = inCubeUV;
	float realY = uv.y;
	uv.y = -abs(realY);

	vec3 camToSphere = normalize(uv * radius);
	float dotValue = dot(camToSphere, lightDir);

	float colorFactor = clamp(dot(vec3(0,1,0), -lightDir), 0.0, 1.0);

	vec3 centerColor = lightColor;
	centerColor.y *= (colorFactor * 1.5);
	centerColor.z *= (colorFactor);

	float alphaSky = -camToSphere.y * 0.6;
	vec3 cubeMapColor = mix(horizonColor, zenitColor, alphaSky);
	
	color = realY < 0 && dotValue > 0.999? vec4(centerColor, 1.0) : vec4(cubeMapColor * colorFactor, 1.0);
	emission = realY < 0 && dotValue > 0.999? vec4(centerColor, 1) : vec4(0,0,0,1);
	godRayInfo = realY < 0 && dotValue > 0.999? vec4(centerColor, 1.0) : vec4(0);
}