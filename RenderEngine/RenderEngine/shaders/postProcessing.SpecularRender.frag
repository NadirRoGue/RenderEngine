#version 330 core

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D postProcessing_0; // Color
uniform sampler2D postProcessing_1; // Normal
uniform sampler2D postProcessing_2; // Specular
uniform sampler2D postProcessing_3; // Emissive
uniform sampler2D postProcessing_4; // z-buffer depth texture
uniform mat4 invProj;

void main()
{
	outColor = vec4(texture(postProcessing_2, texCoord).rgb, 1.0);
}