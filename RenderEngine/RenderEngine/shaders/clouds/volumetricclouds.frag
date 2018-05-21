#version 430 core
layout (location=0) out vec4 color;
layout (location=1) out vec4 depth;
layout (location=2) out vec4 emission;

in vec2 texCoord;

uniform mat4 invView;
uniform mat4 invProj;
uniform mat4 viewProj;

uniform vec3 camPos;

uniform sampler3D perlinworley;
uniform sampler3D worley;
uniform sampler2D weather;


uniform vec3 lightDir;

//uniform float time;

uniform vec3 noiseKernel[6u] = vec3[] 
(
	vec3( 0.38051305,  0.92453449, -0.02111345),
	vec3(-0.50625799, -0.03590792, -0.86163418),
	vec3(-0.32509218, -0.94557439,  0.01428793),
	vec3( 0.09026238, -0.27376545,  0.95755165),
	vec3( 0.28128598,  0.42443639, -0.86065785),
	vec3(-0.16852403,  0.14748697,  0.97460106)
);

uniform vec3 sphereCenter = vec3(0,-40,0);
uniform float sphereRadius = 60.0;

uniform vec3 planeMin = vec3(-200, 30.0, -200);
uniform vec3 planeMax = vec3(200, 70.0, 200);

uniform vec4 STRATUS_GRADIENT = vec4(0.02f, 0.05f, 0.09f, 0.11f);
uniform vec4 STRATOCUMULUS_GRADIENT = vec4(0.02f, 0.2f, 0.48f, 0.625f);
uniform vec4 CUMULUS_GRADIENT = vec4(0.01f, 0.0625f, 0.78f, 1.0f);

// ==========================================================================

float HenyeyGreenstein(vec3 l, vec3 v, float g, float ca)
{
	float g2 = g * g;

	return ((1.0 - g2) / pow((1.0 + g2 - 2.0 * g * ca), 1.5 )) / 4.0 * 3.1415;
}

float Beer(float density)
{
	return exp(-density);
}

float Powder(float density, float ca)
{
	float f = 1.0 - exp(-density * 2.0);
	return mix(1.0, f, clamp(-ca * 0.5 + 0.5, 0.0, 1.0));
}

float LightEnergy(vec3 l, vec3 v, float ca, float coneDensity, float realDensity)
{
	return 2.0 * Beer(coneDensity) * Powder(realDensity, ca) * mix(HenyeyGreenstein(l, v, 0.8, ca), HenyeyGreenstein(l, v, -0.5, ca), 0.5);
}

vec3 ambientLight(float heightFrac)
{
	return mix(vec3(0.5f, 0.67f, 0.7f), vec3(1.0f, 1.0f, 1.0f), heightFrac);
}

// ==========================================================================

float getHeightFraction(vec3 p)
{
	float fraction = (p.y - planeMin.y) / (planeMax.y - planeMin.y);
	//fraction = clamp(fraction, 0.0, 1.0);
	return fraction;
}

vec2 getPlanarUV(vec3 p)
{
	vec2 local = p.xz - planeMin.xz;
	vec2 normalized = local / (planeMax.xz - planeMin.xz);
	return clamp(normalized, 0.0, 1.0);
}

float remapValue(float original, float oMin, float oMax, float nMin, float nMax)
{
	return nMin + ((original - oMin) / (oMax - oMin)) * (nMax - nMin);
}

float getDensityForCloud(vec3 p, float cloudType)
{
	float normalY = getHeightFraction(p);

	//if(normalY > 0.6)
	//	return 0.0;

	return smoothstep(STRATOCUMULUS_GRADIENT.x, STRATOCUMULUS_GRADIENT.y, normalY) - smoothstep(STRATOCUMULUS_GRADIENT.z, STRATOCUMULUS_GRADIENT.w, normalY);
}

float sampleCloudDensity(vec3 p, vec3 weatherData)
{
	vec3 wind = vec3(1,1,0);
	float cloudTopOffset = 30.0;

	float height_fraction = getHeightFraction (p);
	height_fraction = clamp(height_fraction, 0.0, 1.0);

	p += height_fraction * wind * cloudTopOffset;

	vec2 uv = getPlanarUV(p);
	vec3 tex3dsample = vec3(uv * 2.0, height_fraction);

	vec4 pwfbm = textureLod(perlinworley, tex3dsample, 0.0);
	float r = pwfbm.r;

	float lowFreqFBM = ( pwfbm.g * 0.625) + ( pwfbm.b * 0.25 ) + ( pwfbm.a * 0.125 );
	lowFreqFBM = clamp(lowFreqFBM, 0.0, 1.0);
	float baseCloudShape = remapValue(r, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	float densityGradient = remapValue(height_fraction, 0, 0.1, 0, 1) * remapValue(height_fraction, 0.2, 0.3, 1, 0);
	baseCloudShape *= densityGradient;

	float coverage = weatherData.r;// * (1.0 - height_fraction); // XXX
	//coverage = pow(coverage, remapValue(height_fraction, 0.7, 0.8, 1.0, mix(1.0, 0.5, 0.8)));
	float coveragedCloud = remapValue(baseCloudShape, coverage, 1.0, 0.0, 1.0);
	float base_cloud_with_coverage = coveragedCloud * coverage;

	// Build−high frequency Worley noise FBM.
	height_fraction = clamp(getHeightFraction(p), 0.0, 1.0);

	vec3 high_frequency_noises = texture(worley, vec3(uv * 25.0, height_fraction)).rgb;
	float high_freq_FBM = (high_frequency_noises.r * 0.625) + (high_frequency_noises.g * 0.25) + (high_frequency_noises.b * 0.125);
	float high_freq_noise_modifier = mix(high_freq_FBM, 1.0 - high_freq_FBM, clamp(height_fraction * 10.0, 0.0, 1.0));
	//base_cloud_with_coverage = base_cloud_with_coverage - high_freq_noise_modifier * (1.0 - base_cloud_with_coverage);
	float final_cloud = remapValue(base_cloud_with_coverage, high_freq_noise_modifier * 0.2, 1.0, 0.0, 1.0);

	return final_cloud;
}

float raymarchToLight(vec3 pos, vec3 d, float stepSize)
{
	vec3 normLightDir = normalize(lightDir);
	vec3 startPos = pos;
	vec3 rayStep = normLightDir * stepSize;
	float coneRadius = 1.0;
	float coneStep = 1.0/6.0;
	float density = 0.0;
	float coneDensity = 0.0;
	float rcpThickness = 1.0 / (stepSize * 6);

	for(int i = 0; i < 6; i++)
	{
		vec3 posInCone = startPos + coneRadius * noiseKernel[i] * float(i + 1);
		float heightFraction = getHeightFraction(posInCone);
		if(heightFraction <= 1.0)
		{
			vec3 weatherData = texture(weather, getPlanarUV(posInCone)).rgb;
			float cloudDensity = sampleCloudDensity(posInCone, weatherData);
			if(cloudDensity > 0.0)
			{
				density += cloudDensity;
				float transmittance = 1.0 - (density * rcpThickness);
				coneDensity += (cloudDensity * transmittance);
			}
		}

		startPos += rayStep;
		coneRadius += coneStep;
	}

	pos += (rayStep * 8.0);
	float heightFraction = getHeightFraction(pos);
	if(heightFraction <= 1.0)
	{
		vec3 weatherData = texture(weather, getPlanarUV(pos)).rgb;
		float cloudDensity = sampleCloudDensity(pos, weatherData);
		if(cloudDensity > 0.0)
		{
			density += cloudDensity;
			float transmittance = 1.0 - clamp(density * rcpThickness, 0.0, 1.0);
			coneDensity += (cloudDensity * transmittance);
		}
	}

	float ca = dot(normLightDir, d);

	return clamp(LightEnergy(normLightDir, d, ca, coneDensity, density), 0.0, 1.0);
}

float frontToBackRaymarch(vec3 startPos, vec3 endPos, out vec3 color)
{
	vec3 dir = endPos - startPos;
	float density = 0.0;
	float thick = length(dir);
	float rcpThick = 1.0 / thick;
	float delta = planeMax.y - planeMin.y;
	int sampleCount = int(ceil(mix(64.0, 128.0, clamp(thick / delta, 0.0, 1.0))));

	vec3 lightColor = vec3(1,1,1) * clamp(dot(vec3(0,1,0), normalize(lightDir)), 0.0, 1.0) + 0.1;

	vec3 pos = startPos;
	vec3 st = dir / float(sampleCount - 1);
	float stepSize = length(st);
	vec3 viewDir = normalize(dir);

	vec4 result = vec4(0.0);

	for(int i = 0; i < sampleCount; i++)
	{
		vec2 planarSamplePos = getPlanarUV(pos);
		float cloudDensity = sampleCloudDensity(pos, texture(weather, planarSamplePos).rgb); // SAMPLE CLOUD textureSamples

		if(cloudDensity > 0.0)	// IF WE HAVE DENSITY, LAUNCH LIGHT SAMPLING
		{
			density += cloudDensity;
			float transmittance = 1.0 - (density * rcpThick);
			float lightDensity = raymarchToLight(pos, viewDir, stepSize); // SAMPLE LIGHT

			vec4 src = vec4(lightColor * lightDensity + ambientLight(getHeightFraction(pos)), cloudDensity * transmittance); // ACCUMULATE 
			src.rgb *= src.a;
			result = (1.0 - result.a) * src + result;

			if(result.a >= 1.0) // EARLY EXIT ON FULL OPACITY
				break;
		}

		pos += st;
	}
	color = result.xyz;
	return result.a;
}

float backToFrontRaymarch(vec3 startPos, vec3 endPos)
{
	int sample_count = 64;
	vec3 dir = startPos - endPos;

	float thick = length(dir);
	float delta = planeMax.y - planeMin.y;
	sample_count = int(ceil(mix(64.0, 128.0, clamp(thick / delta, 0.0, 1.0))));

	vec3 pos = endPos;
	vec3 st = dir / float(sample_count - 1);

	vec4 result = vec4(0.0);

	float density = 0.0;
	int samplesWithoutDensity = 0;

	for(int i = 0; i < sample_count; i++)
	{
		vec2 planarSamplePos = getPlanarUV(pos);
		float cloudDensity = sampleCloudDensity(pos, texture(weather, planarSamplePos).rgb);

		vec4 src = vec4(1,1,1,cloudDensity);
		//src.a *= 0.5;
		src.rgb *= src.a;
		result = (1.0 - src.a) * result + src;

		if(result.a > 0.95)
			break;

		//result += cloudDensity * stepDt;
		pos += st;
	}

	return result.x;
}

bool IntersectBox(vec3 o, vec3 d, out vec3 minT, out vec3 maxT)
{
	// compute intersection of ray with all six bbox planes
	vec3 invR = 1.0 / d;
	vec3 tbot = invR * (planeMin - o);
	vec3 ttop = invR * (planeMax - o);
	// re-order intersections to find smallest and largest on each axis
	vec3 tmin = min (ttop, tbot);
	vec3 tmax = max (ttop, tbot);
	// find the largest tmin and the smallest tmax
	vec2 t0 = max (tmin.xx, tmin.yz);
	float tnear = max (t0.x, t0.y);
	t0 = min (tmax.xx, tmax.yz);
	float tfar = min (t0.x, t0.y);
	
	// check for hit
	bool hit;
	if ((tnear > tfar) || tnear < 0.0 || tfar < 0.0)
		hit = false;
	else
		hit = true;

	minT = o + d * tnear;
	maxT = o + d * tfar;

	return hit;
}

void main()
{
	// Ray direction
	vec2 fulluv = texCoord * 2.0 - 1.0;
	float z = 1.0 / tan(radians(45.0));
	vec3 viewDir = normalize(vec3(fulluv, -z));
	vec3 worldDir = normalize( (invView * vec4(viewDir, 0)).xyz);
	// Volume intersection points
	vec3 pos, endPos;

	if(IntersectBox(camPos, worldDir, pos, endPos)) 
	{
		vec3 outColor;
		float density = frontToBackRaymarch(pos, endPos, outColor);
		density = clamp(density, 0.0, 1.0);
		
		if(density < 0.01)
			discard;
		
		//density = ;
		color = vec4(outColor, density);
		emission = vec4(0,0,0,0);
	}
	else
	{
		discard;
	}
}