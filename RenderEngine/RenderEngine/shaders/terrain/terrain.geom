#version 400 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

layout (location=0) in vec2 inUV[];
layout (location=1) in float height[];

layout (location=0) out vec2 outUV;
layout (location=1) out vec3 outPos;
layout (location=2) out float outHeight;

uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 normal;

void main()
{
	vec4 a = gl_in[0].gl_Position;
	vec4 b = gl_in[1].gl_Position;
	vec4 c = gl_in[2].gl_Position;
	
	outUV = inUV[0];
	outHeight = height[0];
	gl_Position = modelViewProj * a;
	outPos = (modelView * a).xyz;
	EmitVertex();

	outUV = inUV[2];
	outHeight = height[2];
	gl_Position = modelViewProj * c;
	outPos = (modelView * c).xyz;
	EmitVertex();

	outUV = inUV[1];
	outHeight = height[1];
	gl_Position = modelViewProj * b;
	outPos = (modelView * b).xyz;
	EmitVertex();

	EndPrimitive();
}