#version 450 core
#extension GL_EXT_geometry_shader4: enable

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

//layout (location=0) in vec3 inPos[3];
//layout (location=1) in vec3 inNormal[3];
//layout (location=2) in vec2 inUV[3];

in vec3 vNormal[3];
in vec2 vUV[3];

//layout (location=0) out vec3 outPos;
//layout (location=1) out vec3 outNormal;
//layout (location=2) out vec2 outUV;

out vec3 gPos;
out vec3 gNormal;
out vec2 gUV;

uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 normal;

uniform sampler2D noise;
uniform float TerrainDisplacement = 0.2;

void main()
{
	vec3 a = gl_PositionIn[0].xyz;
	vec3 b = gl_PositionIn[1].xyz;
	vec3 c = gl_PositionIn[2].xyz;

	vec3 aColor = texture(noise, vUV[0]).rgb;
	float aColorFactor = aColor.r;
	vec3 bColor = texture(noise, vUV[1]).rgb;
	float bColorFactor = bColor.r;
	vec3 cColor = texture(noise, vUV[2]).rgb;
	float cColorFactor = cColor.r;

	vec3 newA = a + vNormal[0] * aColorFactor * TerrainDisplacement;
	vec3 newB = b + vNormal[1] * bColorFactor * TerrainDisplacement;
	vec3 newC = c + vNormal[2] * cColorFactor * TerrainDisplacement;

	gNormal = normalize((normal * vec4(vNormal[0], 0.0)).xyz);
	gUV = vUV[0];
	gPos = (modelView * vec4(newA, 1.0)).xyz;
	gl_Position = modelViewProj * vec4(newA, 1.0);
	EmitVertex();

	gNormal = normalize((normal * vec4(vNormal[1], 0.0)).xyz);
	gUV = vUV[1];
	gPos = (modelView * vec4(newB, 1.0)).xyz;
	gl_Position = modelViewProj * vec4(newB, 1.0);
	EmitVertex();

	gNormal = normalize((normal * vec4(vNormal[2], 0.0)).xyz);
	gUV = vUV[2];
	gPos = (modelView * vec4(newC, 1.0)).xyz;
	gl_Position = modelViewProj * vec4(newC, 1.0);
	EmitVertex();

	EndPrimitive();
}