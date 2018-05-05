#version 430 core

#define MASK_MAX_SIZE 64u

// Output
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outDepth;
layout (location=2) out vec4 outEmission;

in vec2 texCoord;

// Input
uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1;
uniform sampler2D postProcessing_2;

uniform vec2 texelSize;

uniform bool horizontal;
uniform bool blend;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
	vec3 result = texture(postProcessing_2, texCoord).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(postProcessing_2, texCoord + vec2(texelSize.x * i, 0.0)).rgb * weight[i];
            result += texture(postProcessing_2, texCoord - vec2(texelSize.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(postProcessing_2, texCoord + vec2(0.0, texelSize.y * i)).rgb * weight[i];
            result += texture(postProcessing_2, texCoord - vec2(0.0, texelSize.y * i)).rgb * weight[i];
        }
    }

	// ================================================================================

	vec4 inColor = texture(postProcessing_0, texCoord);
	vec4 inDepth = texture(postProcessing_1, texCoord);

	vec3 finalColor = blend? inColor.rgb + result : inColor.rgb;

	outColor = vec4(finalColor, 1.0);
	outDepth = inDepth;
	outEmission = vec4(result * 1.12, 1.0);
	gl_FragDepth = inDepth.x;
}