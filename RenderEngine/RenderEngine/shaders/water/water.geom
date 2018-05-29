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

void main()
{
	vec4 a = gl_in[0].gl_Position;
	vec4 b = gl_in[1].gl_Position;
	vec4 c = gl_in[2].gl_Position;
	
	outUV = inUV[0];
	gl_Position = a;
	EmitVertex();

	outUV = inUV[1];
	gl_Position = b;
	EmitVertex();

	outUV = inUV[2];
	gl_Position = c;
	EmitVertex();

	EndPrimitive();
}