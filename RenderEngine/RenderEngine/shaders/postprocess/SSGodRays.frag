#version 430 core

layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outEmission;

in vec2 texCoord;

uniform vec2 lightScreenPos;
uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;
uniform bool onlyPass;

#define NUM_SAMPLES 100

uniform sampler2D postProcessing_0; // color
uniform sampler2D postProcessing_1; // emission
uniform sampler2D postProcessing_2; // god rays

void main()
{
	if(!onlyPass)
	{
		vec2 deltaTextCoord = vec2(texCoord - lightScreenPos);
		vec2 uv = texCoord;
		deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
		float illuminationDecay = 1.0;
	
		vec4 result = texture(postProcessing_2, texCoord);
		for(int i=0; i < NUM_SAMPLES ; i++)
		{
			uv -= deltaTextCoord;
			vec4 sampl = texture(postProcessing_2, uv);
			
			sampl *= illuminationDecay * weight;

			result += sampl;

			illuminationDecay *= decay;
		}
		outColor = texture(postProcessing_0, texCoord) + result * exposure;
		outEmission = texture(postProcessing_1, texCoord);
	}
	else
	{
		outColor = texture(postProcessing_0, texCoord);
		outEmission = texture(postProcessing_1, texCoord);
	}
}