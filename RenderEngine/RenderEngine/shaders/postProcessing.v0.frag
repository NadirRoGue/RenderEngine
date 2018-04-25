#version 330 core

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1;

void main()
{
	outColor = vec4(texture(postProcessing_0, texCoord));
	gl_FragDepth = texture(postProcessing_1, texCoord).r;
}