#version 430 core

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1;

void main()
{
	//float r = texture(postProcessing_0, texCoord).r;
	//outColor = vec4(r, r, r, 1.0);
	outColor = vec4(texture(postProcessing_0, texCoord));
	//gl_FragDepth = texture(postProcessing_1, texCoord).r;
}