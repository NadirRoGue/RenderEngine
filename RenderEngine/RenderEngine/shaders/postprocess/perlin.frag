#version 450 core

in vec2 texCoord;

uniform float amplitude;
uniform float frecuency;
uniform float scale;
uniform float octaves;

layout (location=0) out vec4 color;

float Random2D( in vec2 st ) 
{
    return fract( sin(dot( st.xy, vec2(12.9898,78.233 ) ) ) * 43758.5453123);
}

float NoiseInterpolation( in vec2 i_coord, in float i_size )
{
	vec2 grid = i_coord * i_size;
    
    vec2 randomInput = floor( grid );
    vec2 weights     = fract( grid );
    
    
    float p0 = Random2D( randomInput );
    float p1 = Random2D( randomInput + vec2( 1.0, 0.0  ) );
    float p2 = Random2D( randomInput + vec2( 0.0, 1.0 ) );
    float p3 = Random2D( randomInput + vec2( 1.0, 1.0 ) );
    
    weights = smoothstep( vec2( 0.0, 0.0 ), vec2( 1.0, 1.0 ), weights ); 
    
    return p0 +
           ( p1 - p0 ) * ( weights.x ) +
           ( p2 - p0 ) * ( weights.y ) * ( 1.0 - weights.x ) +
           ( p3 - p1 ) * ( weights.y * weights.x );    
}



void main()
{

    float noiseValue = 0.0;
    
    float localAplitude  = amplitude;
    float localFrecuency = frecuency;

    for( int index = 0; index < octaves; index++ )
    {
     	       
        noiseValue += NoiseInterpolation( texCoord, scale * localFrecuency ) * localAplitude;
    
        localAplitude   /= 2.0;
        localFrecuency  *= 2.0;
    }    

    noiseValue = noiseValue;// * 0.5 + 0.5;

	color = vec4(  vec3( noiseValue ), 1.0 );
}