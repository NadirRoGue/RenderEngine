#version 410 core

layout(triangles) in;
#ifdef WIRE_MODE
layout(line_strip, max_vertices=3) out;
#else
layout(triangle_strip, max_vertices=3) out;
#endif

layout (location=0) in vec2 inUV[];

layout (location=0) out vec2 outUV;
layout (location=1) out vec3 outPos;
layout (location=2) out vec4 outShadowMapPos;

uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 normal;

uniform mat4 lightDepthMat;

void main()
{
	vec4 a = gl_in[0].gl_Position;
	vec4 b = gl_in[1].gl_Position;
	vec4 c = gl_in[2].gl_Position;
	
	outUV = inUV[0];
	outShadowMapPos = lightDepthMat * a;
	gl_Position = modelViewProj * a;
	outPos = (modelView * a).xyz;
	EmitVertex();

	outUV = inUV[2];
	outShadowMapPos = lightDepthMat * c;
	gl_Position = modelViewProj * c;
	outPos = (modelView * c).xyz;
	EmitVertex();

	outUV = inUV[1];
	outShadowMapPos = lightDepthMat * b;
	gl_Position = modelViewProj * b;
	outPos = (modelView * b).xyz;
	EmitVertex();

	EndPrimitive();
}