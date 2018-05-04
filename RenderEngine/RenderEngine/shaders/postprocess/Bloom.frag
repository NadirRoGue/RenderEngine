#version 430 core

#define MASK_MAX_SIZE 64u

// Output
layout (location=0) out vec4 outColor;
layout (location=1) out vec4 outDepth;

// Input
uniform sampler2D postProcessing_0;
uniform sampler2D postProcessing_1;