#version 430 core

#ifndef SHADOW_MAP
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 outSpecular;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outPos;
layout (location=5) out vec4 outInfo;

layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inColor;
layout (location=2) in vec3 inNormal;
layout (location=3) in vec3 inEmission;
layout (location=4) in vec3 inShadowMapPos;
layout (location=5) in vec3 inShadowMapPos1;
layout (location=6) in vec3 inWorldPos;
layout (location=7) in vec2 inNDotAxis;

uniform float Tamplitude = 0.5;
uniform float Tfrecuency = 0.5;
uniform float Tscale = 10.0;
uniform int Toctaves = 5;

uniform sampler2D depthTexture;
uniform sampler2D depthTexture1;

uniform mat4 normal;

uniform vec3 lightDir;

uniform vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

// ================================================================================
float Random2D(in vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float NoiseInterpolation(in vec2 i_coord, in float i_size)
{
	vec2 grid = i_coord *i_size;

	vec2 randomInput = floor(grid);
	vec2 weights = fract(grid);


	float p0 = Random2D(randomInput);
	float p1 = Random2D(randomInput + vec2(1.0, 0.0));
	float p2 = Random2D(randomInput + vec2(0.0, 1.0));
	float p3 = Random2D(randomInput + vec2(1.0, 1.0));

	weights = smoothstep(vec2(0.0, 0.0), vec2(1.0, 1.0), weights);

	return p0 +
		(p1 - p0) * (weights.x) +
		(p2 - p0) * (weights.y) * (1.0 - weights.x) +
		(p3 - p1) * (weights.y * weights.x);
}

float noise(in vec2 pos)
{

	float noiseValue = 0.0;

	float localAplitude = Tamplitude;
	float localFrecuency = Tfrecuency;

	for (int index = 0; index < Toctaves; index++)
	{
		noiseValue += NoiseInterpolation(pos, Tscale * localFrecuency) * localAplitude;

		localAplitude /= 2.0;
		localFrecuency *= 2.0;
	}

	return noiseValue;// * noiseValue * noiseValue * 0.01;
}

bool whithinRange(vec2 texCoord)
{
	return texCoord.x >= 0.0 && texCoord.x <= 1.0 && texCoord.y >= 0.0 && texCoord.y <= 1.0;
}

float getShadowVisibility(vec3 rawNormal)
{
	float bias = clamp(0.005 * tan(acos(dot(rawNormal, lightDir))), 0.0, 0.01);
	float visibility = 1.0;
	if(whithinRange(inShadowMapPos.xy))
	{
		float curDepth = inShadowMapPos.z - bias;
		for (int i = 0; i < 4; i++)
		{
			visibility -= 0.25 * ( texture(depthTexture, inShadowMapPos.xy + poissonDisk[i] / 700.0).x  <  curDepth? 1.0 : 0.0 );
		}
	}
	/*else if(whithinRange(inShadowMapPos1.xy))
	{
		float curDepth = inShadowMapPos1.z - bias;
		visibility = texture(depthTexture1, inShadowMapPos1.xy).x < curDepth? 0.0 : 1.0;
	}*/

	return visibility;
}

bool discardLeaf()
{
	float upSign = sign(inNDotAxis.x);
	float leftSign = sign(inNDotAxis.y);
	float cosUp = abs(inNDotAxis.x);
	float cosRight = abs(inNDotAxis.y);
	vec2 coords;
	if(cosUp < 0.5)
	{
		if(cosRight < 0.5)
		{
			coords = upSign < 0.0? inWorldPos.yz : inWorldPos.zy;
		}
		else
		{
			coords = upSign < 0.0? inWorldPos.yx : inWorldPos.xy;
		}
	}
	else
	{
		coords = leftSign < 0.0? inWorldPos.zx : inWorldPos.xz;
	}

	return noise(coords) < 0.75;
}

#else
layout (location=0) out vec4 lightdepth;
#endif

void main()
{
#ifndef SHADOW_MAP
	vec3 rawNormal = normalize(inNormal);
#ifdef WIRE_MODE
	outColor = vec4(0,0,0,1);
	outNormal = vec4(rawNormal, 0);
	outSpecular = vec4(0,0,0,0);
	outEmissive = vec4(0,0,0,0);
	outPos = vec4(inPos, 1);
	outInfo = vec4(0);
#else
	if(inEmission.x == 1.0 && discardLeaf())
		discard;
	// APPLY SHADOW MAP
	// ------------------------------------------------------------------------------
	float visibility = getShadowVisibility(rawNormal);

	outColor = vec4(inColor, 1.0);
	outNormal = vec4(rawNormal, 1);
	outSpecular = vec4(0,0,0,0);
	outEmissive = vec4(inEmission.y > 0.0? inColor * 0.2 : vec3(0),1);
	outPos = vec4(inPos, 1);
	outInfo = vec4(0.0, visibility,0,1);
#endif
#else
	lightdepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 0);
#endif
}