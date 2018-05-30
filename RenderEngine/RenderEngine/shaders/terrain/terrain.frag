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

uniform mat4 normal;

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
    return 1.0 - dist0;
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

vec2 Curl(float nx1, float nx2, float ny1, float ny2)
{
	float dx = nx1 - nx2;
	float dy = ny1 - ny2;
	return vec2(dy, -dx) * 10.0;
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
	float u = inUV.x;
	float v = inUV.y;
	float step = 0.01;
	float tH = noiseHeight(vec2(u, v + step), scale, octaves); 
	float bH = noiseHeight(vec2(u, v - step), scale, octaves);
	float rH = noiseHeight(vec2(u + step, v), scale, octaves);
	float lH = noiseHeight(vec2(u - step, v), scale, octaves); 

	vec3 rawNormal = normalize(vec3(lH - rH, step * step, bH - tH));

	vec3 up = vec3(0, 1, 0);
	float cosV = abs(dot(rawNormal, up));

	bool sandP = height < waterHeight + 0.01;
	bool rockP = cosV <= grassCoverage;
	if(sandP)
	{
		float bumptH = noiseHeight(vec2(u, v + step), 500.0, 2);
		float bumpbH = noiseHeight(vec2(u, v - step), 500.0, 2);
		float bumprH = noiseHeight(vec2(2 * u + step, v), 500.0, 2);
		float bumplH = noiseHeight(vec2(2 * u - step, v), 500.0, 2);
		vec3 correct = vec3(0, 3.0, 0);
		vec3 bumpNormal = normalize(vec3(bumplH - bumprH, step * step, bumpbH - bumptH));
		rawNormal = correct + bumpNormal;
		rawNormal = normalize(rawNormal);
	}
	else if(rockP)
	{
		float nx1 = noiseHeight(vec2(u - step, v),  200.0, octaves);
		float nx2 = noiseHeight(vec2(u + step, v),  200.0, octaves);
		float ny1 = noiseHeight(vec2(u, v - step),  200.0, octaves);
		float ny2 = noiseHeight(vec2(u, v + step),  200.0, octaves);
		
		vec2 curlV = Curl(nx1, nx2, ny1, ny2) * 10.0;
		vec3 apply = cross(vec3(0,1,0), rawNormal) * (curlV.x - curlV.y); 
		rawNormal += apply;
		rawNormal = normalize(rawNormal);
	}
	

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
	heightColor = height <= waterHeight + 0.01? sand : cosV > grassCoverage? grass : rock;
	
	grassData = heightColor == grass? 1.0 : 0.0;
	// APPLY SHADOW MAP
	// ------------------------------------------------------------------------------
	visibility = getShadowVisibility(rawNormal);

	// Depth for below-water level areas
	alpha = height <= waterHeight? (height / waterHeight) - 0.6 : 1.0;
	alpha = clamp(alpha, 0, 1);
#endif


	// OUTPUT G BUFFERS
	// ------------------------------------------------------------------------------
	outColor = vec4(heightColor, 1.0);
	outNormal = vec4(normalize(n), 1.0);
	outPos = vec4(inPos, 1.0);
	outSpecular = vec4(0,0,0,0);
	outEmissive = vec4(0,0,0,0);
	outInfo = vec4(grassData, visibility, alpha, 0);
#endif
}