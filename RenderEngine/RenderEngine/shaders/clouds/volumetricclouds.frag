#version 430 core

layout (location=0) out vec4 color;

in vec2 texCoord;

// Transform ray from view to world space
uniform mat4 invView;

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

#define CLOUD_TOP_OFFSET 25.0
#define WIND_DIRECTION vec3(1,-0.2,0)

#define SPHERE_PROJECTION

// Cloud layer data
#ifdef SPHERE_PROJECTION
vec3 sphereCenter;
float chunkLen;
#define SPHERE_Y -1900.0
#define SPHERE_INNER_RADIUS 2000.0
#define SPHERE_OUTER_RADIUS 2125.0
//#define SPHERE_Y -8500.0
//#define SPHERE_INNER_RADIUS 8600.0
//#define SPHERE_OUTER_RADIUS 8750.0
#define SPHERE_DELTA float(SPHERE_OUTER_RADIUS - SPHERE_INNER_RADIUS)
#else
uniform vec3 planeMin = vec3(-2000, 30.0, -2000);
uniform vec3 planeMax = vec3(2000, 230.0, 2000);
vec2 planeDim = vec2(planeMax.xz - planeMin.xz);
#endif

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
	return f;
}

// Full cloud light energy equation
float lightEnergy(vec3 l, vec3 v, float ca, float coneDensity)
{
	return 2.0 * beer(coneDensity) * powder(coneDensity, ca) * mix(henyeyGreenstein(l, v, 0.8, ca), henyeyGreenstein(l, v, -0.5, ca), 0.5);
}

// Returns an ambient lighting depending on the height
vec3 ambientLight(float heightFrac, float lightFactor)
{
	return mix(realLightColor * 0.7, realLightColor * 0.9, heightFrac);
}

// ==========================================================================
// Density functions

// Normalized height (0 - 1) within the cloud layer
float getHeightFraction(vec3 p)
{
#ifdef SPHERE_PROJECTION
	float fraction = fract(((p.y) / (SPHERE_DELTA)));
#else
	float fraction = (p.y - planeMin.y) / (planeMax.y - planeMin.y);
#endif

	//fraction = clamp(fraction, 0.0, 1.0);

	return fraction;
}

// Planar uv based on current point position and overall cloud layer size
vec2 getPlanarUV(vec3 p)
{
#ifdef SPHERE_PROJECTION
	vec2 uv = (p.xz - vec2(-SPHERE_OUTER_RADIUS)) / (SPHERE_OUTER_RADIUS * 2.0);
#else
	vec2 uv = (p.xz - planeMin.xz) / planeDim;
#endif
	return uv;
}

// Retrieves the weather data stored in the weather texture
vec3 getWeatherData(vec3 p)
{
	vec2 uv = getPlanarUV(p);
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
	return remapValue(heightFraction, baseGradient.x, baseGradient.y, 0.0, 1.0) * remapValue(heightFraction, baseGradient.z, baseGradient.w, 1.0, 0.0);
}

// Retrieves the density of clouds at a given point
float sampleCloudDensity(vec3 p, vec3 weatherData, float lod, bool expensive, float hf)
{
	// Position modifications
	//float heightFraction = getHeightFraction (p);

	// Make clouds evolve with wind
	p += hf * WIND_DIRECTION * CLOUD_TOP_OFFSET;
	p += WIND_DIRECTION * time * cloudSpeed;

	float heightFraction = getHeightFraction(p);

	// Apply uv (xz) scale to keep clouds un-stretched based on cloud layer thickness
#ifdef SPHERE_PROJECTION
	float uvScale = (SPHERE_OUTER_RADIUS * 2.0) / SPHERE_DELTA;
#else
	float hScale = planeMax.x - planeMin.x;
	float vScale = planeMax.y - planeMin.y;
	uvScale = hScale / (vScale);
#endif

	// Sample base cloud shape noises (Perlin-Worley + 3 Worley)
	vec2 uv = getPlanarUV(p) * uvScale;
	vec4 baseCloudNoise = textureLod(perlinworley, vec3(uv, heightFraction), lod);
	
	// Build the low frequency fbm modifier
	float lowFreqFBM = ( baseCloudNoise.g * 0.625) + ( baseCloudNoise.b * 0.25 ) + ( baseCloudNoise.a * 0.125 );
	float baseCloudShape = remapValue(baseCloudNoise.r * 2.0, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	// Apply density gradient based on cloud type
	float densityGradient = getDensityForCloud(heightFraction, cloudType);
	baseCloudShape *= (densityGradient / heightFraction) * (1.0 - heightFraction);

	// Apply coverage
	float coverage = weatherData.r * coverageMultiplier;
	float coveragedCloud = remapValue(baseCloudShape, coverage, 1.0, 0.0, 1.0);
	coveragedCloud *= coverage;

	float finalCloud = coveragedCloud;

	// Only erode the cloud if visible
	if(expensive)
	{
		// Build−high frequency Worley noise FBM.
		vec3 erodeCloudNoise = textureLod(worley, vec3(uv, heightFraction) * 0.1, lod).rgb;
		float highFreqFBM = (erodeCloudNoise.r * 0.625) + (erodeCloudNoise.g * 0.25) + (erodeCloudNoise.b * 0.125);

		heightFraction = getHeightFraction(p);
		float highFreqNoiseModifier = mix(highFreqFBM, 1.0 - highFreqFBM, clamp(heightFraction * 1.0, 0.0, 1.0));

		coveragedCloud = coveragedCloud - highFreqNoiseModifier * (1.0 - coveragedCloud);

		finalCloud = remapValue(coveragedCloud, highFreqNoiseModifier * 0.2, 1.0, 0.0, 1.0);
	}

	return clamp(finalCloud, 0.0, 1.0);
}

// ==========================================================================
// Raymarchers

float raymarchToLight(vec3 pos, vec3 d, float stepSize)
{
	vec3 startPos = pos;
	vec3 rayStep = lightDir * (stepSize * 5.0);
	float coneRadius = 1.0;
	float density = 0.0;
	float coneDensity = 0.0;
	float invDepth = 1.0 / (stepSize * 6);

	// Light raymarchToLight
	// 6 close samples
	int i = 0;
	while(i < 6)
	{
		vec3 posInCone = startPos + coneRadius * noiseKernel[i] * float(i);
		float heightFraction = getHeightFraction(posInCone);
		if(heightFraction <= 1.0)
		{
			// sample the expensive way if we hare near borders (where density is low, like 0.3 or below)
			float cloudDensity = sampleCloudDensity(posInCone, getWeatherData(posInCone), float(i + 1), coneDensity < 0.3, heightFraction);
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
	float cloudDensity = sampleCloudDensity(pos, getWeatherData(pos), 6.0, false, heightFraction);
	if(cloudDensity > 0.0)
	{
		density += cloudDensity;
		float transmittance = 1.0 - (density * invDepth);
		coneDensity += (cloudDensity * transmittance);
	}

	//float ca = -clamp(dot(lightDir, d), 0.0, 1.0);

	// Compute light energy arriving at point
	return lightEnergy(lightDir, d, 1.0, coneDensity);
}

float frontToBackRaymarch(vec3 startPos, vec3 endPos, out vec3 color)
{
	// Sampling parameters calculation
	vec3 path = endPos - startPos;

#ifdef SPHERE_PROJECTION
	int sampleCount = int(ceil(mix(48.0, 96.0, clamp(length(path) / SPHERE_DELTA, 0.0, 1.0))));
#else
	float delta = planeMax.y - planeMin.y;
	int sampleCount = int(ceil(mix(48.0, 96.0, clamp(length(path) / delta, 0.0, 1.0))));
#endif

	// ambient lighting attenuation factor
	float ambientFactor =  max(min(lightFactor, 1.0), 0.1);
	// full lighting
	vec3 lc = realLightColor * lightFactor * cloudColor;

	// Ray march data
	vec3 pos = startPos;
	vec3 st = path / float(sampleCount - 1);
	float stepSize = length(st);
	vec3 viewDir = normalize(path);
	float density = 0.0;
	vec4 result = vec4(0.0);
	float samplingLod = mix(0.0, 2.0, (length(startPos) - SPHERE_INNER_RADIUS) / (SPHERE_DELTA));

	// Dithering on the starting ray position to reduce banding artifacts
	int a = int(gl_FragCoord.x) % 4;
	int b = int(gl_FragCoord.y) % 4;
	pos += st * bayerFilter[a * 4 + b] * 10.0;

	int i = 0;
	while(i < sampleCount)
	{
		float cloudDensity = sampleCloudDensity(pos, getWeatherData(pos), samplingLod, true, getHeightFraction(pos)); // SAMPLE CLOUD textureSamples

		if(cloudDensity > 0.0)	// IF WE HAVE DENSITY, LAUNCH LIGHT SAMPLING
		{
			density += cloudDensity;
			float lightEnergy = raymarchToLight(pos, viewDir, stepSize); // SAMPLE LIGHT

			float height = getHeightFraction(pos);
			vec4 src = vec4(lc * 0.7 * lightEnergy + ambientLight(height, lightFactor) * ambientFactor * cloudColor, cloudDensity); // ACCUMULATE 
			src.rgb *= src.a;
			result = (1.0 - result.a) * src + result;

			if(result.a >= 0.95) // EARLY EXIT ON FULL OPACITY
				break;
		}

		pos += st;
		i++;
	}

	// Return alpha (density) and store final color
	color = result.xyz;
	return result.a;
}

// ==========================================================================
// Collision detection
#ifdef SPHERE_PROJECTION
bool intersectSphere(vec3 o, vec3 d, out vec3 minT, out vec3 maxT)
{
	// Intersect inner sphere
	vec3 sphereToOrigin = o - sphereCenter;
	float b = dot(d, sphereToOrigin);
	float c = dot(sphereToOrigin, sphereToOrigin);
	float sqrtOpInner = b*b - (c - SPHERE_INNER_RADIUS*SPHERE_INNER_RADIUS);

	// No solution (we are outside the sphere, looking away from it)
	float maxSInner;
	if(sqrtOpInner < 0.0)
	{
		return false;
		//maxSInner = 0.0;
	}
	else
	{
		float deInner = sqrt(sqrtOpInner);
		float solAInner = -b - deInner;
		float solBInner = -b + deInner;

		maxSInner = max(solAInner, solBInner);
		maxSInner = maxSInner < 0.0? 0.0 : maxSInner;
	}
	// Intersect outer sphere
	float sqrtOpOuter = b*b - (c - SPHERE_OUTER_RADIUS*SPHERE_OUTER_RADIUS);

	// No solution - same as inner sphere
	if(sqrtOpOuter < 0.0)
	{
		return false;
	}
	
	float deOuter = sqrt(sqrtOpOuter);
	float solAOuter = -b - deOuter;
	float solBOuter = -b + deOuter;

	float maxSOuter = max(solAOuter, solBOuter);
	maxSOuter = maxSOuter < 0.0? 0.0 : maxSOuter;

	// Compute entering and exiting ray points
	float minSol = min(maxSInner, maxSOuter);
	float maxSol = max(maxSInner, maxSOuter);

	minT = o + d * minSol;
	maxT = o + d * maxSol;

	chunkLen = length(maxT - minT);

	return minT.y > -500.0; // only above the horizon
}
#else 
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
#endif

void main()
{
	// Frag coordinates must be adjusted to perform temporal reprojection
	// We are rendering a half width frame, but the rays direction (based on the screen coordinates)
	// should account for the final result (full hd screen size)
	int frameIter = frame % 2;
	vec2 fragCoord = vec2(gl_FragCoord.x * 2.0 + frameIter, gl_FragCoord.y);
	
	// Do now raymarch the clouds if the fragment is occluded
	if(texture(currentPixelDepth, vec2(fragCoord / screenResolution)).x < 1.0)
	{
		color = vec4(0,0,0,0);
	}
	else
	{
#ifdef SPHERE_PROJECTION
		sphereCenter = vec3(camPos.x, SPHERE_Y, camPos.z);
#endif
		// computing ray direction based on screen coordinates adn camera fov
		vec2 fulluv = fragCoord - screenResolution / 2.0;
		float z =  screenResolution.y / tan(radians(FOV));
		vec3 viewDir = normalize(vec3(fulluv, -z / 2.0));
		vec3 worldDir = normalize( (invView * vec4(viewDir, 0)).xyz);
		// Volume intersection points
		vec3 startPos, endPos;

	#ifdef SPHERE_PROJECTION
		bool intersect = intersectSphere(camPos, worldDir, startPos, endPos);
	#else
		bool intersect = intersectBox(camPos, worldDir, startPos, endPos);
	#endif
	
		if(intersect)
		{
			vec3 outColor;
			// If intersected, raymarch cloud
			float density = frontToBackRaymarch(startPos, endPos, outColor);
			density = clamp(density, 0.0, 1.0);

			vec4 finalColor = vec4(outColor, density);

	#ifdef SPHERE_PROJECTION
			// Apply atmospheric fog to far away clouds
			vec4 ambientColor = vec4(mix(horizonColor, zenitColor, 0.15), 0.6);

			// Use current position distance to center as action radius
			float dist = length(startPos - camPos);
			float radius = (camPos.y - sphereCenter.y) * 0.3;
			float alpha = clamp(dist / radius, 0.0, 1.0);
			finalColor = mix(finalColor, ambientColor * lightFactor, alpha);
	#endif

			color = finalColor;
		}
		else
		{
			//discard;
			color = vec4(0);
		}
	}
}