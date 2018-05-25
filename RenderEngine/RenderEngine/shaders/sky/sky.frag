#version 410 core

layout (location=0) out vec4 color;
//layout (location=1) out vec4 depth;
layout (location=1) out vec4 emission;

layout (location=0) in vec3 inCubeUV;

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

	float colorFactor = clamp(dot(vec3(0,1,0), -lightDir), 0.25, 1.0);//clamp(-lightDir.y, 0.25, 1.0);

	vec3 centerColor = lightColor;
	centerColor.y *= (colorFactor * 1.5);
	centerColor.z *= (colorFactor);

	float alphaSky = -camToSphere.y * 0.6;
	vec3 horizon = vec3(0.8,0.85,1);
	vec3 zenit = vec3(0.4, 0.7, 1);
	vec3 cubeMapColor = mix(horizon, zenit, alphaSky);

	vec3 skyColor;
	
	vec3 emissive;
	if(realY < 0 && dotValue > 0.999)
	{
		skyColor = centerColor;
		emissive = centerColor;
	}
	else
	{
		skyColor = cubeMapColor * colorFactor;
		emissive = vec3(0,0,0);
	}

	color = vec4(skyColor, 1.0);
	//depth = vec4(gl_FragDepth, 0, 0, 1);
	emission = vec4(emissive, 1);
}