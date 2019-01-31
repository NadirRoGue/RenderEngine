#version 430 core

layout (location=0) out vec4 outColor;

layout (location=0) in vec2 texCoord;
layout (location=1) in vec3 planePos;

uniform vec2 screenSize;

uniform sampler2D postProcessing_0;
uniform sampler2D grassBuffer;
uniform sampler2D posBuffer;

// Same remap value function as in the volumetric clouds, returns the valor within a range of 
// a valor which is mapped to a different range
float blend(float val , float val0 , float val1 , float res0 , float res1)
{
	if( val <= val0 ) return res0;
	if( val >= val1 ) return res1;

	return res0 + (val - val0) * (res1 - res0) / (val1 - val0);
}

void main()
{
	float isGrass = texture(grassBuffer, texCoord).x;
	vec4 backColor = texture(postProcessing_0, texCoord);
	// Apply the effect only if we are treating a grass pixel
	if(isGrass > 0.9)
	{
		vec3 pos = texture(posBuffer, texCoord).xyz;
		float dist = length(pos);

		float ar = screenSize.x / screenSize.y;

		float x = planePos.x / (screenSize.x / ar) * ar;
		float y = planePos.y / (screenSize.y * ar) * ar;

		float d = blend(dist, 0, 500, 100, 500);
		float dclose = blend(dist, 0, 1, 100, 2);

		d *= dclose;

		y += x * 1000.0;

		// keep only fractional so we skip some places, creating blades of grass instead of a "wall" of grass
		float yOffset = fract(y * d) / d;
		vec2 uvOffset = texCoord - vec2(0, yOffset * 2.0 * 1.0/(dist * 0.2));

		vec3 offsetPos = texture(posBuffer, uvOffset).xyz;
		// Make sure we dont paint grass in a zone oclude by non-grass data
		outColor = offsetPos.z < pos.z? backColor : vec4(mix(backColor.rgb, texture(postProcessing_0, uvOffset).rgb, clamp(1 - yOffset * d / 3.4, 0, 1)), 1.0);
		//texture(postProcessing_0, uvOffset).rgb
	}
	else
	{
		outColor = backColor;
	}
}