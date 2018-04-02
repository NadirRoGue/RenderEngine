#version 450 core
//#extension GL_ARB_separate_shader_objects : enable

layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 outSpecular;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outPos;

//layout (location=0) in vec3 inPos;
//layout (location=1) in vec3 inNormal;
//layout (location=2) in vec2 inUV;

in vec3 gPos;
in vec3 gNormal;
in vec2 gUV;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
vec3 N;

uniform sampler2D noise;

void main()
{
	Kd = texture(noise, gUV).rgb;//vec3(1.0, 0.0, 0.0);
	Ke = vec3(0,0,0);
	Ks = vec3(0,0,0);
	
	outColor = vec4(Kd, 1.0);
	outNormal = vec4(normalize(gNormal), 1.0);
	outSpecular = vec4(Ks, 1.0);
	outEmissive = vec4(Ke, 1.0);
	outPos = vec4(gPos, 1.0);
}