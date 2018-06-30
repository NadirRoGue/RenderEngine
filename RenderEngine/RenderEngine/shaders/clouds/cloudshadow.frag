#version 430 core

layout (location=0) out vec4 lightDepth;

layout (location=0) in vec3 inPos;

uniform sampler2D weather;
uniform float coverageMultiplier;

#define SPHERE_RADIUS 2125.0 / 2.0

void main()
{
	// Get the planar UV to access the weather texture
	vec2 uv = (inPos.xz + SPHERE_RADIUS) / (SPHERE_RADIUS * 2.0);
	float coverage = texture(weather, uv).r;
	// Modify it with the coverage multiplier
	coverage *= coverage;
	coverage *= coverageMultiplier;

	// If almost 0, discard it and allow light to pass throught
	if(coverage <= 0.0009)
		discard;

	gl_FragDepth = 0.0;
	lightDepth = vec4(0,0,0,1);
}