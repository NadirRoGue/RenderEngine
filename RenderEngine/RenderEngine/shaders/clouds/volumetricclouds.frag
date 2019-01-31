#version 430 core

layout (location=0) out vec4 color;
layout (location=1) out vec4 outPos;

in vec2 texCoord;

// Transform ray from view to world space
uniform mat4 projView;
uniform mat4 invView;
uniform vec3 sphereCenter;
uniform float innerSphereRadius;
uniform float outerSphereRadius;
uniform float maxRenderDist;
uniform float cloudTopOffset;
uniform float weatherScale;
uniform float baseNoiseScale;
uniform float highFreqNoiseScale;
uniform float highFreqNoiseUVScale;
uniform float highFreqNoiseHScale;

uniform vec3 camPos;

uniform sampler2D currentPixelDepth;

// used to adjust u coordinate based on aspect ratio
uniform vec2 screenResolution;
uniform float FOV;

// Noise textures for cloud shapes and erosion
uniform sampler3D perlinworley;
uniform sampler3D worley;
// weather texture
uniform sampler2D weather;

// Light data
uniform vec3 lightDir;
//uniform vec3 lightColor;
uniform vec3 realLightColor;
uniform float lightFactor;

uniform vec3 zenitColor;
uniform vec3 horizonColor;

uniform vec3 cloudColor;

uniform int frame;

// Cloud evolution
uniform float time;
uniform float cloudType;
uniform float cloudSpeed;
uniform vec3 windDirection;
uniform float coverageMultiplier;

// Cone sampling random offsets
uniform vec3 noiseKernel[6u] = vec3[] 
(
	vec3( 0.38051305,  0.92453449, -0.02111345),
	vec3(-0.50625799, -0.03590792, -0.86163418),
	vec3(-0.32509218, -0.94557439,  0.01428793),
	vec3( 0.09026238, -0.27376545,  0.95755165),
	vec3( 0.28128598,  0.42443639, -0.86065785),
	vec3(-0.16852403,  0.14748697,  0.97460106)
);

// Random offset added to starting ray depth to prevent banding artifacts 
#define BAYER_FACTOR 1.0/16.0
uniform float bayerFilter[16u] = float[]
(
	0.0*BAYER_FACTOR, 8.0*BAYER_FACTOR, 2.0*BAYER_FACTOR, 10.0*BAYER_FACTOR,
	12.0*BAYER_FACTOR, 4.0*BAYER_FACTOR, 14.0*BAYER_FACTOR, 6.0*BAYER_FACTOR,
	3.0*BAYER_FACTOR, 11.0*BAYER_FACTOR, 1.0*BAYER_FACTOR, 9.0*BAYER_FACTOR,
	15.0*BAYER_FACTOR, 7.0*BAYER_FACTOR, 13.0*BAYER_FACTOR, 5.0*BAYER_FACTOR
);

// Cloud types height density gradients
#define STRATUS_GRADIENT vec4(0.0, 0.1, 0.2, 0.3)
#define STRATOCUMULUS_GRADIENT vec4(0.02, 0.2, 0.48, 0.625)
#define CUMULUS_GRADIENT vec4(0.00, 0.1625, 0.88, 0.98)

// Light sampling constants
#define CONE_STEP 0.1666666

// ==========================================================================
// Lighting functions
// Scattering phase function
float henyeyGreenstein(vec3 l, vec3 v, float g, float ca)
{
	float g2 = g * g;

	return ((1.0 - g2) / pow((1.0 + g2 - 2.0 * g * ca), 1.5 )) * (1.0 / (4.0 * 3.1415));
}

// Intensity decreases with density
float beer(float density)
{
	return exp(-density);
}

// Beer's law inverted equation for edges
float powder(float density, float ca)
{
	float f = 1.0 - exp(-density * 2.0);
	return mix(1.0, f, clamp(-ca * 0.5 + 0.5, 0.0, 1.0));
	//return f;
}

// Full cloud light energy equation
float lightEnergy(vec3 l, vec3 v, float ca, float coneDensity)
{
	return 50.0 * beer(coneDensity) * powder(coneDensity, ca) * henyeyGreenstein(l, v, 0.2, ca);
}

// Returns an ambient lighting depending on the height
vec3 ambientLight()
{
	vec3 lightColor = realLightColor;
	vec3 ambientColor = mix(horizonColor, zenitColor, 0.15);
	return mix(lightColor, ambientColor, 0.65) * lightFactor * 0.65;
}

// ==========================================================================
// Density functions

// Normalized height (0 - 1) within the cloud layer
float getHeightFraction(vec3 p)
{
	return (length(p - sphereCenter) - innerSphereRadius) / (outerSphereRadius - innerSphereRadius);
}

vec2 sphericalUVProj(vec3 p)
{
	vec3 dirVector = normalize(p - sphereCenter);
	//return vec2(0.5 + atan(dirVector.x, abs(dirVector.y))/(2.0 * 3.14159), 0.5 + (asin(dirVector.z) / 3.14159));
	return (dirVector.xz + 1.0) / 2.0;
}

// Retrieves the weather data stored in the weather texture
vec3 getWeatherData(vec3 p)
{
	vec2 uv = sphericalUVProj(p) * weatherScale;
	return texture(weather, uv).rgb;
}

// Remap utility
float remapValue(float original, float oMin, float oMax, float nMin, float nMax)
{
	return nMin + ((original - oMin) / (oMax - oMin)) * (nMax - nMin);
}

// Retrieves the cloud density based on cloud type and weighting between default cloud models
float getDensityForCloud(float heightFraction, float cloudType)
{
	float stratusFactor = 1.0 - clamp(cloudType * 2.0, 0.0, 1.0);
	float stratoCumulusFactor = 1.0 - abs(cloudType - 0.5) * 2.0;
	float cumulusFactor = clamp(cloudType - 0.5, 0.0, 1.0) * 2.0;

	vec4 baseGradient = stratusFactor * STRATUS_GRADIENT + stratoCumulusFactor * STRATOCUMULUS_GRADIENT + cumulusFactor * CUMULUS_GRADIENT;

	// gradicent computation (see Siggraph 2017 Nubis-Decima talk)
	float result = remapValue(heightFraction, baseGradient.x, baseGradient.y, 0.0, 1.0) * remapValue(heightFraction, baseGradient.z, baseGradient.w, 1.0, 0.0);
	//result = exp(result);
	return result;
}

// Retrieves the density of clouds at a given point
float sampleCloudDensity(vec3 p, float lod, bool expensive, float heightFraction)
{
	//lod = 0.0;
	//expensive = true;
	// Make clouds evolve with wind
	p += heightFraction * windDirection * cloudTopOffset;
	p += windDirection * time * cloudSpeed;

	float deltaDist = clamp(length(p - camPos) / maxRenderDist, 0.0, 1.0);

	vec3 weatherData = getWeatherData(p);

	vec2 uv = sphericalUVProj(p);

	// Sample base cloud shape noises (Perlin-Worley + 3 Worley)
	vec4 baseCloudNoise = textureLod(perlinworley, vec3(uv * baseNoiseScale, heightFraction), lod);
	
	// Build the low frequency fbm modifier
	float lowFreqFBM = ( baseCloudNoise.g * 0.625) + ( baseCloudNoise.b * 0.25 ) + ( baseCloudNoise.a * 0.125 );
	float baseCloudShape = remapValue(baseCloudNoise.r, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	// Apply density gradient based on cloud type
	float densityGradient = getDensityForCloud(heightFraction, weatherData.g);
	baseCloudShape *= densityGradient;

	// Apply coverage
	float coverage = clamp(weatherData.r, 0.0, 1.0) * coverageMultiplier;
	// Make sure cloud with less density than actual coverage dissapears
	float coveragedCloud = remapValue(baseCloudShape, coverage, 1.0, 0.0, 1.0);
	coveragedCloud *= coverage;
	coveragedCloud *= mix(1.0, 0.0, clamp(heightFraction / coverage, 0.0, 1.0));

	float finalCloud = coveragedCloud;

	// Only erode the cloud if erosion will be visible (low density sampled until now)
	if(expensive)
	{
		// Build−high frequency Worley noise FBM.
		vec3 erodeCloudNoise = textureLod(worley, vec3(uv * highFreqNoiseUVScale, heightFraction * highFreqNoiseHScale) * highFreqNoiseScale, lod).rgb;
		float highFreqFBM = (erodeCloudNoise.r * 0.625) + (erodeCloudNoise.g * 0.25) + (erodeCloudNoise.b * 0.125);

		// Recompute height fraction after applying wind and top offset
		heightFraction = getHeightFraction(p);

		float highFreqNoiseModifier = mix(highFreqFBM, 1.0 - highFreqFBM, clamp(heightFraction * 8.5, 0.0, 1.0));
		//coveragedCloud = coveragedCloud - highFreqNoiseModifier * (1.0 - coveragedCloud);
		finalCloud = remapValue(coveragedCloud, highFreqNoiseModifier * 0.8, 1.0, 0.0, 1.0);
	}

	return clamp(finalCloud, 0.0, 1.0);
}

// ==========================================================================
// Raymarchers

float raymarchToLight(vec3 pos, vec3 d, float stepSize)
{
	vec3 startPos = pos;
	// Modify step size to take as much info as possible in 6 steps
	vec3 rayStep = lightDir * (stepSize) * 0.7;
	// Starting cone radius. Will increase as we move along it
	float coneRadius = 1.0;
	// Sampled density until now
	float invDepth = 1.0 / (stepSize * 6);
	float density = 0.0;
	// Density - transmittance until now
	float coneDensity = 0.0;

	// Light raymarchToLight
	// 6 close samples
	int i = 0;
	while(i < 6)
	{
		// Get position inside cone
		vec3 posInCone = startPos + lightDir + coneRadius * noiseKernel[i] * float(i);

		float deltaDist = clamp(length(posInCone - camPos) / maxRenderDist, 0.0, 1.0);

		// By advancing towards the light we might go outside the atmosphere
		float heightFraction = getHeightFraction(posInCone);
		if(heightFraction <= 1.0)
		{
			// sample the expensive way if we hare near borders (where density is low, like 0.3 or below)
			float cloudDensity = sampleCloudDensity(posInCone, float(i + 1), coneDensity < 0.3, heightFraction);
			if(cloudDensity > 0.0)
			{
				density += cloudDensity;
				float transmittance = 1.0 - (density * invDepth);
				coneDensity += (cloudDensity * transmittance);
			}
		}

		startPos += rayStep;
		coneRadius += CONE_STEP;
		i++;
	}

	// 1 far sample for shadowing
	pos += (rayStep * 8.0);
	float heightFraction = getHeightFraction(pos);
	float cloudDensity = sampleCloudDensity(pos, 6.0, false, heightFraction);
	if(cloudDensity > 0.0)
	{
		density += cloudDensity;
		float transmittance = 1.0 - (density * invDepth);
		coneDensity += (cloudDensity * transmittance);
	}

	float ca = dot(lightDir, d);

	// Compute light energy arriving at point
	return lightEnergy(lightDir, d, ca, coneDensity);
}

float frontToBackRaymarch(vec3 startPos, vec3 endPos, out vec3 color)
{
	// Sampling parameters calculation
	vec3 path = endPos - startPos;

	int sampleCount = int(ceil(mix(48.0, 96.0, clamp((length(path) / (outerSphereRadius - innerSphereRadius)), 0.0, 1.0))));

	// Ray march data
	vec3 stepVector = path / float(sampleCount - 1);
	float stepSize = length(stepVector);
	vec3 viewDir = normalize(path);

	vec3 pos = startPos;
	float density = 0.0;
	vec4 result = vec4(0.0);

	float lodAlpha = clamp(length(startPos - camPos) / maxRenderDist, 0.0, 1.0);
	float samplingLod = mix(0.0, 6.0, lodAlpha);
	//(length(startPos) - innerSphereRadius) / (outerSphereRadius - innerSphereRadius));

	// Dithering on the starting ray position to reduce banding artifacts
	int a = int(gl_FragCoord.x) % 4;
	int b = int(gl_FragCoord.y) % 4;
	pos += stepVector * bayerFilter[a * 4 + b];

	// ambient lighting attenuation factor
	float ambientFactor =  max(min(lightFactor, 1.0), 0.1);
	// full lighting
	vec3 lc = realLightColor * lightFactor * cloudColor;
	vec3 ambientL = ambientLight();// vec3(0.8) * lightFactor;

	int i = 0;
	while(i < sampleCount)
	{
		float cloudDensity = sampleCloudDensity(pos, samplingLod, true, getHeightFraction(pos)); // SAMPLE CLOUD textureSamples

		if(cloudDensity > 0.0)	// IF WE HAVE DENSITY, LAUNCH LIGHT SAMPLING
		{
			density += cloudDensity;
			float lightEnergy = raymarchToLight(pos, viewDir, stepSize); // SAMPLE LIGHT

			float height = getHeightFraction(pos);
			vec4 src = vec4(lc * lightEnergy + ambientL, cloudDensity); // ACCUMULATE 
			src.rgb *= src.a;
			result = (1.0 - result.a) * src + result;

			if(result.a >= 0.95) // EARLY EXIT ON FULL OPACITY
				break;
		}

		pos += stepVector;
		i++;
	}

	// Return alpha (density) and store final color
	color = result.xyz;
	return result.a;
}

// ==========================================================================
// Collision detection
bool intersectSphere(vec3 o, vec3 d, out vec3 minT, out vec3 maxT)
{
	// Intersect inner sphere
	vec3 sphereToOrigin = (o - sphereCenter);
	float b = dot(d, sphereToOrigin);
	float c = dot(sphereToOrigin, sphereToOrigin);
	float sqrtOpInner = b*b - (c - innerSphereRadius * innerSphereRadius);

	// No solution (we are outside the sphere, looking away from it)
	float maxSInner;
	if(sqrtOpInner < 0.0)
	{
		return false;
	}
	
	float deInner = sqrt(sqrtOpInner);
	float solAInner = -b - deInner;
	float solBInner = -b + deInner;

	maxSInner = max(solAInner, solBInner);

	if(maxSInner < 0.0) return false;

	maxSInner = maxSInner < 0.0? 0.0 : maxSInner;
	
	// Intersect outer sphere
	float sqrtOpOuter = b*b - (c - outerSphereRadius * outerSphereRadius);

	// No solution - same as inner sphere
	if(sqrtOpOuter < 0.0)
	{
		return false;
	}
	
	float deOuter = sqrt(sqrtOpOuter);
	float solAOuter = -b - deOuter;
	float solBOuter = -b + deOuter;

	float maxSOuter = max(solAOuter, solBOuter);

	if(maxSOuter < 0.0) return false;

	maxSOuter = maxSOuter < 0.0? 0.0 : maxSOuter;

	// Compute entering and exiting ray points
	float minSol = min(maxSInner, maxSOuter);

	if(minSol > maxRenderDist)
	{
		return false;
	}

	float maxSol = max(maxSInner, maxSOuter);

	minT = o + d * minSol;
	maxT = o + d * maxSol;

	return true;
}

void main()
{
	// Frag coordinates must be adjusted to perform temporal reprojection
	// We are rendering a half width frame, but the rays direction (based on the screen coordinates)
	// should account for the final result (full hd screen size)
	int frameIter = frame % 2;

	float coordX = gl_FragCoord.x;
	float coordY = gl_FragCoord.y;

#define INTERLEAVED
#ifdef INTERLEAVED
	vec2 fragCoord = vec2(coordX * 2.0 + frameIter, coordY);
#else
	vec2 fragCoord = vec2(coordX + (screenResolution.x / 2.0) * frameIter, coordY);
#endif
	// Do now raymarch the clouds if the fragment is occluded
	if(texture(currentPixelDepth, vec2(fragCoord / screenResolution)).x < 1.0)
	{
		color = vec4(0);
		outPos = vec4(0,0,0,1);
	}
	else
	{
		// computing ray direction based on screen coordinates adn camera fov
		vec2 fulluv = fragCoord - screenResolution / 2.0;
		float z =  screenResolution.y / tan(radians(FOV));
		vec3 viewDir = normalize(vec3(fulluv, -z / 2.0));
		vec3 worldDir = normalize( (invView * vec4(viewDir, 0)).xyz);
		// Volume intersection points
		vec3 startPos, endPos;

		bool intersect = intersectSphere(camPos, worldDir, startPos, endPos);
	
		vec4 ambientColor = vec4(mix(horizonColor, zenitColor, 0.15), 0.6);
		if(intersect)
		{
			vec3 outColor = vec3(0);
			// If intersected, raymarch cloud
			float density = frontToBackRaymarch(startPos, endPos, outColor);
			density = clamp(density, 0.0, 1.0);

			vec4 finalColor = vec4(outColor, density);
			float alpha = length(startPos - camPos) / maxRenderDist;
			alpha *= alpha;
			alpha = clamp(alpha, 0, 1);
			finalColor = mix(finalColor, ambientColor * lightFactor, alpha);
			color = finalColor;
			outPos = vec4(startPos, 1.0);//vec4(camPos + normalize(startPos - camPos) * 100.0, 1.0);
		}
		else
		{
			color = ambientColor * lightFactor;
			outPos = vec4(0,0,0,1);
		}
	}
}