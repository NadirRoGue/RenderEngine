#version 430 core

#define maskSize 9u
#define maskFactor 1.0/14.0

layout (location=0) out vec4 outColor;

layout (location=0) in vec2 texCoord;

uniform sampler2D postProcessing_0;
uniform sampler2D depthBuffer;		

uniform vec2 texelSize;

uniform float focalDistance;
uniform float maxDistanceFactor;

uniform mat4 invProj;

uniform vec2 affectedTexels[maskSize] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,0.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));

uniform float kernel[maskSize] = float[](
	1.0*maskFactor, 2.0*maskFactor, 1.0*maskFactor,
	2.0*maskFactor, 2.0*maskFactor, 2.0*maskFactor,
	1.0*maskFactor, 2.0*maskFactor, 1.0*maskFactor); 

void main()
{
	vec4 color = vec4 (0.0);

	float zBufferDepth = texture(depthBuffer, texCoord).x;
	zBufferDepth = zBufferDepth * 2 - 1; 

	vec4 temp = vec4(texCoord, zBufferDepth, 1);
	vec4 cameraCords = invProj * temp;
	
	float zCam = -(cameraCords.z / cameraCords.w);

	float dof = abs(zCam + focalDistance) * maxDistanceFactor;

	dof = clamp (dof, 0.0, 1.0);
	dof *= dof * dof;

	for (uint i = 0u; i < maskSize; i++)
	{
		vec2 iidx = texCoord + texelSize * affectedTexels[i] * dof;
		float curDepth = texture(depthBuffer, iidx).x;
		color += texture(postProcessing_0, iidx, 0.0) * kernel[i] * (1.0 - abs(curDepth - zBufferDepth));
	}

	outColor = color;
}