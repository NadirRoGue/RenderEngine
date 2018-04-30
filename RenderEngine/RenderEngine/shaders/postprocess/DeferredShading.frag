#version 330 core

layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outDepth;

in vec2 texCoord;

uniform sampler2D postProcessing_0; // color
uniform sampler2D postProcessing_1; // normal
uniform sampler2D postProcessing_2; // specular
uniform sampler2D postProcessing_3; // emissive
uniform sampler2D postProcessing_4; // pos
uniform sampler2D postProcessing_5; // depth

// ===============================================
uniform vec3 backgroundColor;

//Propiedades del objeto
vec3 pos;
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 Ke;
vec3 N;
float alpha = 100.0;

// Point light
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 lpos; 
uniform vec3 PLattenuation;

// Spot light
uniform vec3 SLIa;
uniform vec3 SLId;
uniform vec3 SLIs;
uniform vec3 SLpos;
uniform vec3 SLdir;
uniform float SLapperture;
uniform float SLm;
uniform vec3 SLattenuation;

// Directional light
uniform vec3 DLIa;
uniform vec3 DLId;
uniform vec3 DLIs;
uniform vec3 DLdir; 

// ====================================================

vec3 shade();

void main()
{
	vec4 originalColor = texture(postProcessing_0, texCoord);
	Ka = Kd = originalColor.xyz;
	N = texture(postProcessing_1, texCoord).xyz;
	Ks = texture(postProcessing_2, texCoord).xyz;
	Ke = texture(postProcessing_3, texCoord).xyz;
	pos = texture(postProcessing_4, texCoord).xyz;

	outColor = vec4(shade(), originalColor.w);
	float depth = texture(postProcessing_5, texCoord).x;
	outDepth = vec4(depth, 0, 0, 1);
	gl_FragDepth = depth;
}

vec3 shade()
{
	vec3 c = vec3(0.0);

	// ============================================
	// Point light
	// ============================================
	c = Ia * Ka;

	vec3 PL_L = lpos - pos;
	float PL_dist = length(PL_L);

	float PL_fatt = min(1/(PLattenuation.x + PLattenuation.y * PL_dist + PLattenuation.z * PL_dist * PL_dist), 1);

	vec3 L = normalize (PL_L);
	vec3 diffuse = Id * Kd * dot (L,N) * PL_fatt;
	c += clamp(diffuse, 0.0, 1.0);
	
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float PL_factor = max (dot (R,V), 0.01);
	vec3 specular = Is*Ks*pow(PL_factor,alpha);
	c += clamp(specular, 0.0, 1.0);
	
	// ============================================
	// Spot light
	// ============================================
	c+= SLIa * Ka;

	vec3 SL_L = SLpos - pos;
	float SL_dist = length(SL_L);
	SL_L = normalize(SL_L);

	// Factor de atenuación
	float SL_fatt = min(1/(SLattenuation.x + SLattenuation.y * SL_dist + SLattenuation.z * SL_dist * SL_dist),1);

	vec3 normDir = normalize(SLdir);

	float cosApertura = cos(radians(SLapperture));
	float point = dot(normDir , -SL_L);

	float factor = max(pow(max(point - cosApertura, 0) / (1 - cosApertura), SLm), 0);

	c += clamp(factor * Kd * SLId * dot(SL_L, N) * SL_fatt, 0, 1);

	vec3 SL_R = normalize(reflect(-SL_L, N));
	float SL_Factor = max(dot(SL_R, V),0.01);
	c += SLIs * Ks * SL_Factor * factor * SL_fatt;

	// ============================================
	// Directional light
	// ============================================
	c += DLIa * Ka;

	vec3 D_L = DLdir; // normalized in the client (see StandarProgram::onDirectionalLightRender)
	
	// LUZ DIFUSA
	c += (DLId * Kd * max(dot(N,D_L), 0));

	vec3 DL_R = normalize(reflect(-D_L, N));
	vec3 DL_V = normalize(-pos);
	float DL_Factor = max(dot(DL_R,DL_V), 0.01);
	c += DLIs * Ks * pow(DL_Factor, alpha);

	c += Ke;

	float d = length(pos);
	float alfa = 1/exp(0.0025*d*d);

	//alfa*I+(1-alfa)*cf;

	return mix(backgroundColor, c, alfa);
}