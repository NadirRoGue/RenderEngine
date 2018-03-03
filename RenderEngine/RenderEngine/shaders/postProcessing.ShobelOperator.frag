#version 330 core

#define MASK_MAX_SIZE 64u

//Color de salida
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outDepth;

//Variables Variantes
in vec2 texCoord;

//Textura
uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1;

uniform vec2 texelSize;

// Masks are fixed, there is no need to uplaod them from the shader
// Declared as uniform and initialized in the shader
uniform const vec2 texIdx[9] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));

uniform const float Gx[9] = float[](
	float (1.0), float (0.0), float (-1.0),
	float (2.0), float (0.0), float (-2.0),
	float (1.0), float (0.0), float (-1.0)
); 

uniform const float Gy[9] = float[](
	float (1.0), float (2.0), float (1.0),
	float (0.0), float (0.0), float (0.0),
	float (-1.0), float (-2.0), float (-1.0)
);

void main()
{
	vec4 color = vec4 (0.0);

	vec3 vertical = vec3(0);
	vec3 horizontal = vec3(0);

	for (uint i = 0u; i < 9u; i++)
	{
		vec2 iidx = texCoord + texelSize * texIdx[i];
		vertical += texture(postProcessing_0, iidx,0.0).xyz * Gy[i];
		horizontal += texture(postProcessing_0, iidx,0.0).xyz * Gx[i];
	}

	vec3 temp = sqrt((vertical * vertical) + (horizontal * horizontal));
	float c = length(temp);

	outColor = c > 0.85? vec4(0,0,0,1) : vec4(texture(postProcessing_0, texCoord).rgb, 1);
	outDepth = vec4(texture(postProcessing_1, texCoord).x, 0, 0, 1); // Already normalized in the deferred shading shader
}