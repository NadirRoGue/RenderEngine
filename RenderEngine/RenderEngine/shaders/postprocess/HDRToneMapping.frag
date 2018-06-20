#version 430 core

layout (location=0) out vec4 outColor;

layout (location=0) in vec2 texCoord;

uniform sampler2D postProcessing_0;

uniform float exposure;
uniform float gamma;

void main()
{
    vec3 hdrColor = texture(postProcessing_0, texCoord).rgb;
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    outColor = vec4(mapped, 1.0);
}