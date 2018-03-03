#version 330 core

#define MASK_MAX_SIZE 64

//Color de salida
out vec4 outColor;

//Variables Variantes
in vec2 texCoord;

//Textura
uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1;

uniform uint maskSize;
uniform vec2 texelSize;
uniform vec2 affectedTexels[MASK_MAX_SIZE];
uniform float kernel[MASK_MAX_SIZE];

uniform float focalDistance;
uniform float maxDistanceFactor;

void main()
{
	vec4 color = vec4 (0.0);

	float dof = abs(texture(postProcessing_1, texCoord).z -focalDistance) * maxDistanceFactor;
	dof = clamp (dof, 0.0, 1.0);
	dof *= dof;

	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + texelSize * affectedTexels[i] * dof;
		color += texture(postProcessing_0, iidx,0.0) * kernel[i];
	}
	outColor = color;
}