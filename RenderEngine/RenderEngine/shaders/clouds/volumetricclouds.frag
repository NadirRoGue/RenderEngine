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

uniform float CLOUD_SCALE = 10.0;
uniform float COVERAGE_THRESHOLD = 0.3;
uniform float COVERAGE_MAX = 0.9;

uniform vec3 sphereCenter = vec3(0,-40,0);
uniform float sphereRadius = 60.0;

uniform vec3 planeMin = vec3(-200, 30.0, -200);
uniform vec3 planeMax = vec3(200, 130.0, 200);

uniform vec4 STRATUS_GRADIENT = vec4(0.0, 0.1, 0.2, 0.3);
uniform vec4 STRATOCUMULUS_GRADIENT = vec4(0.02, 0.2, 0.48, 0.625);
uniform vec4 CUMULUS_GRADIENT = vec4(0.01, 0.0625, 0.78, 1.0);

// ==========================================================================
// Lighting functions
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
	return f;
}

float LightEnergy(vec3 l, vec3 v, float ca, float coneDensity, float realDensity)
{
	return 2.0 * Beer(coneDensity) * Powder(realDensity, ca) * (1.0/3.1415) * mix(HenyeyGreenstein(l, v, 0.8, ca), HenyeyGreenstein(l, v, -0.5, ca), 0.5);
}

vec3 ambientLight(float heightFrac)
{
	return mix(vec3(0.3f, 0.47f, 0.5f), vec3(0.7f, 0.7f, 0.7f), heightFrac);
}

// ==========================================================================
// Density functions
float getHeightFraction(vec3 p)
{
	float fraction = (p.y - planeMin.y) / (planeMax.y - planeMin.y);
	fraction = clamp(fraction, 0.0, 1.0);
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

float getDensityForCloud(float heightFraction, float cloudType)
{
	float stratusFactor = 1.0 - clamp(cloudType * 2.0, 0.0, 1.0);
	float stratoCumulusFactor = 1.0 - abs(cloudType - 0.5) * 2.0;
	float cumulusFactor = clamp(cloudType - 0.5, 0.0, 1.0) * 2.0;

	vec4 baseGradient = stratusFactor * STRATUS_GRADIENT + stratoCumulusFactor * STRATOCUMULUS_GRADIENT + cumulusFactor * CUMULUS_GRADIENT;

	return remapValue(heightFraction, baseGradient.x, baseGradient.y, 0.0, 1.0) * remapValue(heightFraction, baseGradient.z, baseGradient.w, 1.0, 0.0);
}

float sampleCloudDensity(vec3 p, vec3 weatherData, float lod, bool expensive)
{
	
	// Position modifications
	vec3 wind = vec3(1,0,1);
	float cloudTopOffset = 10.0;
	float heightFraction = getHeightFraction (p);

	float cloudType = 1.0;

	p += heightFraction * wind * cloudTopOffset;

	float hScale = planeMax.x - planeMin.x;
	float vScale = planeMax.y - planeMin.y;

	float uvScale = hScale / vScale;

	// Sample base cloud shape noises (Perlin-Worley + 3 Worley)
	vec2 uv = getPlanarUV(p) * uvScale;
	vec3 tex3dsample = vec3(uv, heightFraction);
	vec4 baseCloudNoise = textureLod(perlinworley, tex3dsample, lod);
	
	// Build the low frequency fbm modifier
	float lowFreqFBM = ( baseCloudNoise.g * 0.625) + ( baseCloudNoise.b * 0.25 ) + ( baseCloudNoise.a * 0.125 );
	float baseCloudShape = remapValue(baseCloudNoise.r, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	// Apply density gradient based on cloud type
	float densityGradient = getDensityForCloud(heightFraction, cloudType);
	baseCloudShape *= densityGradient / heightFraction;

	// Apply coverage
	float coverageStart = weatherData.r;
	float coverageEnd = weatherData.g;
	float coverageV = mix(coverageStart, 0.0, heightFraction);
	float coverage = coverageV * 0.5;// * (1.0 - heightFraction);
	//coverage = pow(coverage, remapValue(heightFraction, 0.7, 0.8, 1.0, mix(1.0, 0.5, 1.0)));
	float coveragedCloud = remapValue(baseCloudShape, coverage, 1.0, 0.0, 1.0);
	coveragedCloud *= coverage;

	float finalCloud = coveragedCloud;

	if(expensive)
	{
		// Build−high frequency Worley noise FBM.
		vec3 erodeCloudNoise = textureLod(worley, vec3(uv, heightFraction) * 0.1, lod).rgb;
		float highFreqFBM = (erodeCloudNoise.r * 0.625) + (erodeCloudNoise.g * 0.25) + (erodeCloudNoise.b * 0.125);

		heightFraction = getHeightFraction(p);
		float highFreqNoiseModifier = mix(highFreqFBM, 1.0 - highFreqFBM, clamp(heightFraction * 10.0, 0.0, 1.0));

		coveragedCloud = coveragedCloud - highFreqNoiseModifier * (1.0 - coveragedCloud);

		finalCloud = remapValue(coveragedCloud, highFreqNoiseModifier * 0.2, 1.0, 0.0, 1.0);
	}

	return clamp(finalCloud, 0.0, 1.0);
}

// ==========================================================================
// Raymarchers

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
			float cloudDensity = sampleCloudDensity(posInCone, weatherData, float(i), true);
			if(cloudDensity > 0.0)
			{
				density += cloudDensity;
				//float transmittance = 1.0 - (density * rcpThickness);
				coneDensity += (cloudDensity);// * transmittance);
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
		float cloudDensity = sampleCloudDensity(pos, weatherData, 6.0, true);
		if(cloudDensity > 0.0)
		{
			density += cloudDensity;
			//float transmittance = 1.0 - clamp(density * rcpThickness, 0.0, 1.0);
			coneDensity += (cloudDensity);// * transmittance);
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

	vec3 lightColor = vec3(3,3,3) * clamp(dot(vec3(0,1,0), normalize(lightDir)), 0.0, 1.0) + 0.01;

	vec3 pos = startPos;
	vec3 st = dir / float(sampleCount - 1);
	float stepSize = length(st);
	vec3 viewDir = normalize(dir);

	//bool startCheck = false;
	//int consecutiveZero = 0;

	vec4 result = vec4(0.0);

	for(int i = 0; i < sampleCount; i++)
	{
		vec2 planarSamplePos = getPlanarUV(pos);
		vec3 weatherData = texture(weather, planarSamplePos).rgb;
		float cloudDensity = sampleCloudDensity(pos, weatherData, 0.0, true); // SAMPLE CLOUD textureSamples

		if(cloudDensity > 0.0)	// IF WE HAVE DENSITY, LAUNCH LIGHT SAMPLING
		{
			//startCheck = true;
			density += cloudDensity;
			float lightEnergy = raymarchToLight(pos, viewDir, stepSize); // SAMPLE LIGHT

			float height = getHeightFraction(pos);
			vec4 src = vec4(lightColor * 0.7 * lightEnergy + ambientLight(height), cloudDensity); // ACCUMULATE 
			src.rgb *= src.a;
			result = (1.0 - result.a) * src + result;

			if(result.a >= 1.0) // EARLY EXIT ON FULL OPACITY
				break;
		}
		//else if(startCheck)
		//{
		//	consecutiveZero++;
		//	if(consecutiveZero > 5)
		//		break;
		//}

		pos += st;
	}
	color = result.xyz;
	return result.a;
}

// ==========================================================================
// Collision detection

bool intersectBox(vec3 o, vec3 d, out vec3 minT, out vec3 maxT)
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
	if ((tnear > tfar) || tfar < 0.0)
		hit = false;
	else
		hit = true;

	minT = tnear < 0.0? o : o + d * tnear; // if we are inside the bb, start point is cam pos
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
	vec3 startPos, endPos;

	if(intersectBox(camPos, worldDir, startPos, endPos)) 
	{
		vec3 outColor;
		float density = frontToBackRaymarch(startPos, endPos, outColor);
		density = clamp(density, 0.0, 1.0);
		
		color = vec4(outColor, density);
		emission = vec4(0,0,0,0);
	}
	else
	{
		discard;
	}
}