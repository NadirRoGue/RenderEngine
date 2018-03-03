#version 330 core

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D postProcessing_0; // z-buffer depth texture
uniform mat4 invProj;

void main()
{
	vec4 color = vec4 (0.0);

	float zBufferDepth = texture(postProcessing_0, texCoord).x;

	float depthColor = (zBufferDepth * zBufferDepth);
	
	outColor = vec4(depthColor,depthColor,depthColor, 1.0);
}