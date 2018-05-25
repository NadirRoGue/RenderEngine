#version 330 core

#define MASK_MAX_SIZE 64u

//Color de salida
out vec4 outColor;

//Variables Variantes
in vec2 texCoord;

//Textura
uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1; // Z-buffer texture

uniform uint maskSize;
uniform vec2 texelSize;
uniform vec2 affectedTexels[MASK_MAX_SIZE];
uniform float kernel[MASK_MAX_SIZE];

uniform float focalDistance;
uniform float maxDistanceFactor;

uniform mat4 invProj;

void main()
{
	vec4 color = vec4 (0.0);

	float zBufferDepth = texture(postProcessing_1, texCoord).x;
	zBufferDepth = zBufferDepth * 2 - 1; 

	vec4 temp = vec4(texCoord,zBufferDepth,1);
	vec4 cameraCords = invProj * temp;
	
	float zCam = -(cameraCords.z / cameraCords.w);

	float dof = abs(zCam + focalDistance) * maxDistanceFactor;

	dof = clamp (dof, 0.0, 1.0);
	dof *= dof;

	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + texelSize * affectedTexels[i] * dof;
		color += texture(postProcessing_0, iidx, 0.0) * kernel[i];
	}
	outColor = color;
}