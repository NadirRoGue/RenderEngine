#version 330
#extension GL_EXT_geometry_shader4: enable

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

layout(location=2) in vec3 inNormal[3];
layout(location=3) in vec2 inUV[3];

layout (location=1) out vec3 outPos;
layout (location=2) out vec3 outNormal;
layout (location=3) out vec2 outUV;

uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

uniform float DisplacementFactor = 1.0;
uniform sampler2D DisplacementTexture;

void main()
{
	vec3 a = gl_PositionIn[0].xyz;
	vec3 b = gl_PositionIn[1].xyz;
	vec3 c = gl_PositionIn[2].xyz;

	float aColorFactor = texture(DisplacementTexture, inUV[0]).r;
	float bColorFactor = texture(DisplacementTexture, inUV[1]).r; 
	float cColorFactor = texture(DisplacementTexture, inUV[2]).r;

	outNormal = (normal * vec4(inNormal[0], 0.0)).xyz;
	outUV = inUV[0];
	vec3 displaced = vec3(a + inNormal[0] * aColorFactor * DisplacementFactor);
	outPos = (modelView * vec4(displaced, 1.0)).xyz;
	gl_Position = modelViewProj * vec4(displaced, 1.0);
	EmitVertex();

	outNormal = (normal * vec4(inNormal[1], 0.0)).xyz;
	outUV = inUV[1];
	displaced = vec3(b + inNormal[1] * bColorFactor * DisplacementFactor);
	outPos = (modelView * vec4(displaced, 1.0)).xyz;
	gl_Position = modelViewProj * vec4(displaced, 1.0);
	EmitVertex();

	outNormal = (normal * vec4(inNormal[2], 0.0)).xyz;
	outUV = inUV[2];
	displaced = vec3(c + inNormal[2] * cColorFactor * DisplacementFactor);
	outPos = (modelView * vec4(displaced, 1.0)).xyz;
	gl_Position = modelViewProj * vec4(displaced, 1.0);
	EmitVertex();

	EndPrimitive();
}