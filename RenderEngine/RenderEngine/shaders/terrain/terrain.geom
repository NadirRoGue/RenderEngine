#version 410 core

layout(triangles) in;
#if defined WIRE_MODE
layout(line_strip, max_vertices=3) out;
#elif defined POINT_MODE
layout(points, max_vertices=3) out;
#else
layout(triangle_strip, max_vertices=3) out;
#endif

layout (location=0) in vec2 inUV[];
layout (location=1) in float height[];

layout (location=0) out vec2 outUV;
layout (location=1) out vec3 outPos;
layout (location=2) out float outHeight;
layout (location=3) out vec4 outShadowMapPos;
layout (location=4) out vec4 outShadowMapPos1;

uniform mat4 modelView;
uniform mat4 modelViewProj;

uniform float waterHeight;

uniform mat4 lightDepthMat;
uniform mat4 lightDepthMat1;

vec3 computeTangent(int m, int a, int b)
{
	vec2 st1 = inUV[a] - inUV[m];
	vec2 st2 = inUV[b] - inUV[m];
	vec3 dBA = gl_in[a].gl_Position.xyz - gl_in[m].gl_Position.xyz;
	vec3 dCA = gl_in[b].gl_Position.xyz - gl_in[m].gl_Position.xyz;

	float div = 1.0 / (st1.x * st2.y - st1.y * st2.x);
	vec2 row = vec2(st2.y, -st1.y);

	vec2 column1 = vec2(dBA.x, dCA.x);
	vec2 column2 = vec2(dBA.y, dCA.y);
	vec2 column3 = vec2(dBA.z, dCA.z);

	vec3 tangent = div * vec3(dot(row, column1), dot(row, column2), dot(row, column3));

	return normalize(tangent);
}

void main()
{
	vec4 a = gl_in[0].gl_Position;
	vec4 b = gl_in[1].gl_Position;
	vec4 c = gl_in[2].gl_Position;

	outUV = inUV[0];
	outHeight = height[0];
	outShadowMapPos = lightDepthMat * a;
	outShadowMapPos1 = lightDepthMat1 * a;
	gl_Position = modelViewProj * a;
	outPos = (modelView * a).xyz;
#ifdef POINT_MODE
	gl_PointSize = min(length(outPos) / 10.0, 0.01);
#endif
	EmitVertex();

	outUV = inUV[1];
	outHeight = height[1];
	outShadowMapPos = lightDepthMat * b;
	outShadowMapPos1 = lightDepthMat1 * b;
	gl_Position = modelViewProj * b;
	outPos = (modelView * b).xyz;
#ifdef POINT_MODE
	gl_PointSize = min(length(outPos) / 10.0, 0.01);
#endif
	EmitVertex();

	outUV = inUV[2];
	outHeight = height[2];
	outShadowMapPos = lightDepthMat * c;
	outShadowMapPos1 = lightDepthMat1 * c;
	gl_Position = modelViewProj * c;
	outPos = (modelView * c).xyz;
#ifdef POINT_MODE
	gl_PointSize = min(length(outPos) / 10.0, 0.01);
#endif
	EmitVertex();

	EndPrimitive();
}