#version 430 core

in vec2 texCoord;

layout (location=0) out vec4 result;

uniform int drawMode;
uniform int layer3D;

// ===========================================================================

// COMMON
float Random2D( in vec2 st ) 
{
    return fract( sin(dot( st.xy, vec2(12.9898,78.233 ) ) ) * 43758.5453123);
}

// ===========================================================================
// WORLEY NOISE SPECIFIC

uniform float worleyscale = 8.0;
uniform int worleyOctaves = 3;
uniform float worleyAmplitude = 1.0;
uniform float worleyFreq;

float WorleyNoise(vec2 uv, float startFreq)
{
	vec2 offset = vec2( -1.0, -1.0 );
    
    float total = 0.0;

	float localFreq = startFreq;
	float localAmp = worleyAmplitude;
    
    for(int i = 0; i < worleyOctaves; i++)
    {
        vec2 currentPos = uv * worleyscale * localFreq; 
    	vec2 gridCoord  = floor( currentPos );
        float dist0 = 1000.0;
        
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
        
        total += (0.7 - dist0 * dist0) * localAmp;
        localAmp *= 0.25;
        localFreq *= 3.0; 
    }
    
   return total;
}

// ===========================================================================

float getPelinWorleyFBM(vec2 uv)
{
	return 0.0;
}

void main()
{
	if(drawMode == 0) // Perlin-Worley FBM + 3 Worley at increasing frequencies
	{
		gl_Layer = layer3D;

		float pwfbm = getPelinWorleyFBM(texCoord);
		float worley1 = WorleyNoise(texCoord, worleyFreq);
		float worley2 = WorleyNoise(texCoord, worleyFreq + 0.5);
		float worley3 = WorleyNoise(texCoord, worleyFreq + 1.0);

		result = vec4(pwfbm, worley1, worley2, worley3);
	}
	else if(drawMode == 1) // Worley FBM at increasing frequencies
	{
	
	}
	else if(drawMode == 2) // Curl Noise
	{
	
	}
	else if(drawMode == 3) // Weather texture
	{

	}
}