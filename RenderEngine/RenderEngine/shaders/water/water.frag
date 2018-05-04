#version 410 core

#ifndef SHADOW_MAP
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outNormal;
layout (location=2) out vec4 outSpecular;
layout (location=3) out vec4 outEmissive;
layout (location=4) out vec4 outPos;

layout (location=0) in vec2 inUV;
layout (location=1) in vec3 inPos;
layout (location=2) in vec4 inShadowMapPos;

uniform mat4 normal;

uniform sampler2D depthTexture;
uniform vec3 lightDir;
vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

uniform ivec2 gridPos;

uniform float time;
uniform vec3 watercolor;
uniform float waterspeed;

// ================================================================================

uniform float scale = 200.0;

float Random2D(in vec2 st)
{
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float cellularNoise(vec2 uv)
{	
	//obtenemos su coordenada en el grid y su coordenada real
    vec2 currentPos = uv * scale; 
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
    
    //dist0 = 1.0 - dist0;
    dist0 = dist0 * 0.5 + 0.5;
	//dist0 = clamp(dist0, 0.3, 1.0);
    return dist0;
}

// ================================================================================

uniform float amplitude = 0.5;
uniform float frecuency = 1.0;
uniform int octaves = 4;

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

float noiseHeight(in vec2 pos)
{

	float noiseValue = 0.0;

	float localAplitude = amplitude;
	float localFrecuency = frecuency;

	for (int index = 0; index < octaves; index++)
	{

		noiseValue += NoiseInterpolation(pos + time * waterspeed, scale * localFrecuency) * localAplitude;
		noiseValue += NoiseInterpolation(pos.yx - time * waterspeed, scale * localFrecuency) * localAplitude;

		localAplitude /= 2.0;
		localFrecuency *= 2.0;
	}

	return noiseValue * 0.000001;
}
#else
layout (location=0) out vec4 lightdepth;
#endif

// ================================================================================

void main()
{
#ifdef SHADOW_MAP
	lightdepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1);
#else
	float u = inUV.x;
	float v = inUV.y;

	// COMPUTE NORMAL
	// ------------------------------------------------------------------------------
	float step = 0.001;
	float tH = noiseHeight(vec2(u, v + step)); 
	float bH = noiseHeight(vec2(u, v - step));
	float rH = noiseHeight(vec2(u + step, v)); 
	float lH = noiseHeight(vec2(u - step, v));

	vec3 rawNormal = normalize(vec3(lH - rH, step * step, bH - tH));

	// Correct normal if we have pass from +X to -X, from +Z to -Z, viceversa, or both
	int xSign = sign(gridPos.x);
	int ySign = sign(gridPos.y);
	rawNormal.x = xSign != 0 ? rawNormal.x * xSign : rawNormal.x;
	rawNormal.z = ySign != 0 ? rawNormal.z * ySign : rawNormal.z;
	vec3 n = normalize((normal * vec4(rawNormal, 0.0)).xyz);
	
	// COMPUTE COLOR
	// ------------------------------------------------------------------------------
#ifdef WIRE_MODE
	vec3 color = vec3(0);
#else
	vec3 color = watercolor;
#endif

	// APPLY SHADOW MAP
	// ------------------------------------------------------------------------------
	float bias = clamp(0.005 * tan(acos(dot(rawNormal, lightDir))), 0.0, 0.01);
	float curDepth = inShadowMapPos.z - bias;
	float visibility = 1.0;
	if(inShadowMapPos.x >= 0 && inShadowMapPos.x <= 1 && inShadowMapPos.y >= 0 && inShadowMapPos.y <= 1)
	{
		for (int i = 0; i < 4; i++)
		{
			visibility -= 0.25 * ( texture( depthTexture, inShadowMapPos.xy + poissonDisk[i] / 700.0 ).x  <  curDepth? 1.0 : 0.0 );
		}
	}

	// OUTPUT TO G-BUFFERS
	// ------------------------------------------------------------------------------
	outColor = vec4(color, visibility);
	outNormal = vec4(n, 1.0);
	outPos = vec4(inPos, 1.0);
	outSpecular = vec4(1,1,1,1);
	outEmissive = vec4(color,0);
#endif
}