#version 410 core

#ifndef SHADOW_MAP
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 outSpecular;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outPos;
layout (location=5) out vec4 outInfo;

layout (location=0) in vec2 inUV;
layout (location=1) in vec3 inPos;
layout (location=2) in float height;
layout (location=3) in vec4 inShadowMapPos;
layout (location=4) in vec4 inShadowMapPos1;
layout (location=5) in vec3 tangentVector;

uniform mat4 normal;
uniform mat4 modelView;

uniform float worldScale;
uniform float renderRadius;

uniform sampler2D depthTexture;
uniform sampler2D depthTexture1;

uniform vec3 lightDir;
uniform vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

uniform vec3 rock;
uniform vec3 grass;
uniform vec3 sand;

uniform float time;

uniform float waterHeight;

uniform ivec2 gridPos;

uniform float grassCoverage;

uniform float amplitude;
uniform float frecuency;
uniform float scale;
uniform int octaves;

// ================================================================================
float Random2D(in vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

//uniform float cellularScale = 1500.0;

float cellularNoise(vec2 uv, float cellularScale)
{	
	//obtenemos su coordenada en el grid y su coordenada real
    vec2 currentPos = uv * cellularScale; 
    vec2 gridCoord  = floor( currentPos );
    
	float dist0 = 1000.0;
    vec2 offset = vec2( -1.0, -1.0 );
    
    //recorremos los vecinos buscando la distancia mas cortas
    for( offset.y = -1.0f; offset.y <= 1.0f; offset.y += 1.0f )
    {
        for( offset.x = -1.0f; offset.x <= 1.0f; offset.x += 1.0f )
        {
            vec2 cellCenter = gridCoord + offset;
            cellCenter      = cellCenter + Random2D( cellCenter );
            
            float dist = length( cellCenter - currentPos );
            
            if( dist < dist0 )
            {
                dist0 = dist;
            }
        }
    }
    
	dist0 = clamp(dist0, 0, 1);
    return dist0 * dist0;
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

float noiseHeight(in vec2 pos, float localScale, int octaveCount)
{

	float noiseValue = 0.0;

	float localAplitude = amplitude;
	float localFrecuency = frecuency;

	for (int index = 0; index < octaveCount; index++)
	{
		noiseValue += NoiseInterpolation(pos, localScale * localFrecuency) * localAplitude;

		localAplitude /= 2.0;
		localFrecuency *= 2.0;
	}

	return noiseValue * noiseValue * noiseValue * 0.01;
}

float bumpNoiseHeight(in vec2 pos, float localScale, int octaveCount)
{
	float noiseValue = 0.0;

	float localAplitude = amplitude;
	float localFrecuency = frecuency;

	for (int index = 0; index < octaveCount; index++)
	{

		noiseValue += NoiseInterpolation(pos, localScale * localFrecuency) * localAplitude;
		noiseValue += NoiseInterpolation(pos.yx, localScale * localFrecuency) * localAplitude;

		localAplitude /= 2.0;
		localFrecuency *= 2.0;
	}

	return noiseValue * 0.001;
}

// =====================================================================
// Shadow map look up
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
		//visibility = texture(depthTexture, inShadowMapPos.xy).x < curDepth? 0.0 : 1.0;
		
		for (int i = 0; i < 4; i++)
		{
			visibility -= 0.25 * ( texture(depthTexture, inShadowMapPos.xy + poissonDisk[i] / 700.0).x  <  curDepth? 1.0 : 0.0 );
		}
		
	}
	else if(whithinRange(inShadowMapPos1.xy))
	{
		float curDepth = inShadowMapPos1.z - bias;
		//visibility = texture( depthTexture1, inShadowMapPos1.xy).x  <  curDepth? 0.0 : 1.0;
		
		for (int i = 0; i < 4; i++)
		{
			visibility -= 0.25 * ( texture( depthTexture1, inShadowMapPos1.xy + poissonDisk[i] / 700.0 ).x  <  curDepth? 1.0 : 0.0 );
		}
		
	}

	return visibility;
}

vec3 computeNormal()
{
	float u = inUV.x;
	float v = inUV.y;
	float step = 0.01;
	float tH = noiseHeight(vec2(u, v + step), scale, octaves); 
	float bH = noiseHeight(vec2(u, v - step), scale, octaves);
	float rH = noiseHeight(vec2(u + step, v), scale, octaves);
	float lH = noiseHeight(vec2(u - step, v), scale, octaves); 

	return normalize(vec3(lH - rH, step * step, bH - tH));
}

vec3 computeBumpNormal()
{
	float u = inUV.x;
	float v = inUV.y;
	float step = 0.001;
	float slope = 2.0;
	float tH = bumpNoiseHeight(vec2(u, v + step), scale * slope, octaves); 
	float bH = bumpNoiseHeight(vec2(u, v - step), scale * slope, octaves);
	float rH = bumpNoiseHeight(vec2(u + step, v), scale * slope, octaves);
	float lH = bumpNoiseHeight(vec2(u - step, v), scale * slope, octaves); 

	return normalize(vec3(lH - rH, step * step, bH - tH));
}

vec3 computeCaustics(vec2 uv)
{
	return vec3(cellularNoise(uv, 150.0));
}

#else
layout (location=0) out vec4 lightdepth;
#endif

// ================================================================================

void main()
{
#ifdef SHADOW_MAP
	lightdepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 0);
#else
	// COMPUTE NORMAL FROM HEIGHTMAP
	// ------------------------------------------------------------------------------
	// Compute vertex normal
	vec3 rawNormal = computeNormal();
	vec3 up = vec3(0, 1, 0);
	float cosV = abs(dot(rawNormal, up));

	// Compute bump normal
	rawNormal = length(inPos) < float(renderRadius * worldScale) / 1.5? computeBumpNormal() : rawNormal;

	// Correct normal if we have pass from +X to -X, from +Z to -Z, viceversa, or both
	int xSign = sign(gridPos.x);
	int ySign = sign(gridPos.y);
	rawNormal.x = xSign != 0 ? rawNormal.x * xSign : rawNormal.x;
	rawNormal.z = ySign != 0 ? rawNormal.z * ySign : rawNormal.z;
	vec3 n = (normal * vec4(rawNormal, 0.0)).xyz;

	// COMPUTE COLOR
	// ------------------------------------------------------------------------------
	vec3 heightColor = vec3(0);
	float visibility = 1.0;
	float grassData = 0.0;
	float alpha = 1.0;
#ifndef WIRE_MODE

	// Compute color gradient based on height / slope
	float tenPerCentGrass = grassCoverage - grassCoverage * 0.1;
	heightColor = height <= waterHeight + 0.01? sand : height <= waterHeight + 0.015? mix(sand, grass, (height - waterHeight - 0.01) / 0.005) : cosV > grassCoverage? grass : cosV > tenPerCentGrass? mix(rock, vec3(0.15,0.1,0.05), (cosV - tenPerCentGrass) / (grassCoverage * 0.1)) : rock;
	
	grassData = heightColor == grass? 1.0 : 0.0;
	// APPLY SHADOW MAP
	// ------------------------------------------------------------------------------
	visibility = getShadowVisibility(rawNormal);

	// Depth for below-water level areas
	alpha = height <= waterHeight? (height / waterHeight) - 0.4 : 1.0;
	alpha = clamp(alpha, 0.0, 1.0);
	heightColor = alpha < 0.95? heightColor + (computeCaustics(inUV + time * 0.007) + computeCaustics(inUV.yx - time * 0.007)) * (0.95 - alpha) * 1.0 : heightColor;
#endif

	// OUTPUT G BUFFERS
	// ------------------------------------------------------------------------------
	outColor = vec4(heightColor, 1.0);
	outNormal = vec4(normalize(n), 1.0);
	outPos = vec4(inPos, 1.0);
	outSpecular = vec4(0);
	outEmissive = vec4(0,0,0,0);
	outInfo = vec4(grassData, visibility, alpha, 0);
#endif
}