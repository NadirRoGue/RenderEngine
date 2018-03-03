#version 330 core

#define MASK_MAX_SIZE 64u

//Color de salida
layout (location=0) out vec4 outColor;

//Variables Variantes
in vec2 texCoord;

//Textura
uniform sampler2D postProcessing_0;

uniform uint maskSize;
uniform vec2 texelSize;
uniform vec2 affectedTexels[MASK_MAX_SIZE];
uniform float kernel[MASK_MAX_SIZE];

void main()
{
	vec4 color = vec4 (0.0);

	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + texelSize * affectedTexels[i];
		color += texture(postProcessing_0, iidx,0.0) * kernel[i];
	}
	outColor = color;
}