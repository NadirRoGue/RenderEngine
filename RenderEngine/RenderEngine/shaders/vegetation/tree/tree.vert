#version 430 core

layout (location=0) in vec3 inPos;	
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inNormal;
layout (location=3) in vec3 inEmission;
layout (location=4) in vec2 inTexCoord;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outEmission;
layout(location = 3) out vec2 outTexCoord;

// Wind data
uniform float sinTime;
uniform vec3 windDirection;
uniform float windStrength;
uniform vec2 tileUV;

void main()
{
	// Make wind direction y 0 to avoid stretching and squashing on the trees
	vec3 wd = vec3(windDirection.x, 0, windDirection.z);

	// Modify base pos by the wind dir/strength, vertex height and some randomness
	vec3 pos = inPos + sinTime * 0.01 * wd * windStrength * inPos.y;

	outColor = inColor;
	outEmission = inEmission;
	outNormal = inNormal;
	outTexCoord = inTexCoord;

	gl_Position = vec4(pos, 1);
}
