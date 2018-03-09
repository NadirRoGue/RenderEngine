//#define ALBEDO_MAPPING
//#define NORMAL_MAPPING
//#define SPECULAR_MAPPING
//#define OCCLUSION_MAPPING
//#define EMISSIVE_MAPPING
//#define DISPLACEMENT_MAPPING
//#define AUTOLOD

#version 330 core

layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inUV;

#ifndef ALBEDO_MAPPING
layout (location=3) in vec3 inColor;
#endif

#ifdef NORMAL_MAPPING
layout (location=4) in vec3 inTangent;
#endif

#ifndef AUTOLOD
#ifndef DISPLACEMENT_MAPPING
uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;
#endif
#endif

#ifndef AUTOLOD
#ifndef DISPLACEMENT_MAPPING
layout (location=1) out vec3 outPos;
#endif
#endif

layout (location=2) out vec3 outNormal;
layout (location=3) out vec2 outUV;

#ifndef ALBEDO_MAPPING
layout (location=4) out vec3 outColor;
#endif

#ifdef NORMAL_MAPPING
layout (location=5) out vec3 outTangent;
#endif

void main()
{
	#ifndef AUTOLOD
	#ifndef DISPLACEMENT_MAPPING
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
	outPos = (modelView * vec4(inPos, 1.0)).xyz;
	outNormal = (normal * vec4(inNormal, 0.0)).xyz;
	#else
	gl_Position = vec4(inPos, 1.0);
	outNormal = inNormal;
	#endif
	#else
	gl_Position = vec4(inPos, 1.0);
	outNormal = inNormal;
	#endif

	outUV = inUV;

	#ifndef ALBEDO_MAPPING
	color = inColor;
	#endif

	#ifdef NORMAL_MAPPING
	outTangent = (modelView * vec4(inTangent,1.0)).xyz;
	#endif
}
