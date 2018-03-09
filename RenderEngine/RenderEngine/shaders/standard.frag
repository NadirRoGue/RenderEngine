#version 330 core

layout (location=0) out vec4 outColor;		// RGB = Color, A = SpecularIntensity
layout (location=1) out vec4 outNormal;		// RGB = Normal, A = SpecularPower
layout (location=2) out vec4 outEmissive;	// RGB = Emission, A = Occlusion
layout (location=3) out vec4 outPos;		// RGB = Position

layout(location=1) in vec3 inPos;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec2 inUV;

#ifndef ALBEDO_MAPPING
layout(location=4) in vec3 inColor;
#endif

#ifdef NORMAL_MAPPING
layout(location=5) in vec3 inTangent;
#endif

#ifdef ALBEDO_MAPPING
uniform sampler2D colorTex;
#endif

#ifdef EMMISIVE_MAPPING
uniform sampler2D emiTex;
#endif

#ifdef SPECULAR_MAPPING
uniform sampler2D specTex;
#endif

#ifdef NORMAL_MAPPING
uniform sampler2D normalTex;
#endif

#ifdef OCCLUSION_MAPPING
uniform sampler2D occlusionTex;
#endif

uniform float SpecularCoefficent;

#ifdef NORMAL_MAPPING
vec3 computeBumpMapping()
{
	vec3 normalizedNormal = normalize(norm);

	vec3 normalizedTangent = normalize(tangent);
	normalizedTangent = normalize(normalizedTangent - (dot(normalizedTangent, normalizedNormal) * normalizedNormal));

	vec3 bitangent = normalize(cross(normalizedNormal, normalizedTangent));

	mat3 tangentBasis = mat3(normalizedTangent, bitangent, normalizedNormal);

	vec3 bumpMapNormal = (((texture(normalTex, inUV).rgb) * 2) - 1);

	return normalize(tangentBasis * bumpMapNormal);
}
#endif

void main()
{
	#ifdef ALBEDO_MAPPING
	vec3 color = texture(colorTex, inUV).rgb;
	#else
	vec3 color = inColor;
	#endif

	#ifdef NORMAL_MAPPING
	vec3 normal = computeBumpMapping();
	#else
	vec3 normal = inNormal;
	#endif

	#ifdef SPECULAR_MAPPING
	float SpecularIntensity = texture(specularTex, inUV).r;
	#else
	float SpecularIntensity = 1.0;
	#endif

	#ifdef EMISSIVE_MAPPING
	vec3 emission = texture(emiTex, inUV).rgb;
	#else
	vec3 emission = ve3(0,0,0);
	#endif

	#ifdef OCCLUSION_MAPPING
	float occlusionFactor = texture(occlusionTex, inUV).r;
	#else
	float occlusionFactor = 1.0;
	#endif


	outColor = vec4(color, SpecularIntensity);
	outNormal = vec4(normal, SpecularCoefficent);
	outEmissive = vec4(emission, occlusionFactor);
	outPos = vec4(inPos, 1.0);
}