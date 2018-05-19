#version 430 core
layout (location=0) out vec4 color;
layout (location=1) out vec4 depth;
layout (location=2) out vec4 emission;

in vec2 texCoord;

uniform mat4 invView;
uniform mat4 invProj;
uniform mat4 viewProj;

uniform sampler3D perlinworley;
uniform sampler3D worley;
uniform sampler2D weather;

uniform vec2 cminmax = vec2(20, 35);

uniform vec3 lightPos = vec3(50,50,0);

//uniform vec3 sunPosition = vec3(40,40,0);

//uniform float time;

//uniform vec3 noiseKernel[7];

uniform vec3 sphereCenter = vec3(0,-40,0);
uniform float sphereRadius = 60.0;

uniform vec2 planeMin = vec2(-100,-100);
uniform vec2 planeMax = vec2(100,100);
uniform vec2 planeSize = vec2(200,200);

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
// ==========================================================================

float getHeightFraction(vec3 p)
{
	float fraction = (p.y - 20.0) / 10.0;
	fraction = clamp(fraction, 0.0, 1.0);
	return fraction;
}

vec2 getWeatherTexCoord(vec3 p)
{
	p = normalize(p);
	float u = atan(p.x, p.z) / (2.0 * 3.1415) + 0.5;
	float v = asin(p.y) / 3.1415 + 0.5;
	return vec2(u, v);
}

vec2 getWeatherTexCoordForPlane(vec3 p)
{
	vec2 local = p.xz - planeMin;
	vec2 normalized = local / planeSize;
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
	vec3 wind_direction = vec3(1.0, 0.0, 0.0);
	float height_fraction = getHeightFraction (p);
	float cloud_top_offset = 1.0;

	// Skew in wind direction .
	p += height_fraction * wind_direction * cloud_top_offset;
	p /= 30.0;
	vec4 pwfbm = texture(perlinworley, p);

	float lowFreqFBM = ( pwfbm.g * 0.625) + ( pwfbm.b * 0.25 ) + ( pwfbm.a * 0.125 );
	float baseCloudShape = remapValue(pwfbm.r, -(1.0 - lowFreqFBM), 1.0, 0.0, 1.0);

	float cloudHeightDensity = getDensityForCloud(p, 0.0);
	//baseCloudShape *= cloudHeightDensity;

	float coverage = weatherData.r * 2.0;

	float coveragedCloud = remapValue(baseCloudShape, 1.0 - coverage, 1.0, 0.0, 1.0);

	float base_cloud_with_coverage = coveragedCloud * coverage;
	return base_cloud_with_coverage;
	vec3 high_frequency_noises = texture(worley, p * 0.1).rgb;
	
	// Build−high frequency Worley noise FBM.
	float high_freq_FBM = (high_frequency_noises.r * 0.625) + (high_frequency_noises.g * 0.25) + (high_frequency_noises.b * 0.125);

	float high_freq_noise_modifier = mix(high_freq_FBM, 1.0 - high_freq_FBM, clamp(height_fraction * 1.0, 0.0, 1.0));
	
	float final_cloud = remapValue(base_cloud_with_coverage, high_freq_noise_modifier * 0.2, 1.0, 0.0, 1.0);

	return final_cloud;
}
/*
float SampleCoudDensityAlongCone(vec3 p,  vec3 d, float stepSize, float originalDensity)
{
	vec3 l = normalize(lightPos - p);
	float ca = dot(normalize(d), l);

	vec3 light_step = l * stepSize;
	float coneR = 1.0;
	float density_along_cone = 0.0;
	float rcpThickness = 1.0 / (stepSize * 6.0);
	float density = 0.0;

	for(int i = 0; i < 6; i++)
	{
		vec3 cp = p + coneR + kernel[i] * float(i + 1);

		float hf = getHeightFraction(p);
		if(hf <= 1.0)
		{
			float tempDensity = sampleCloudDensity(p, texture(weather, getWeatherTexCoordForPlane(p)).rgb);
			if(tempDensity > 0.0)
			{
				density += tempDensity;
				float transmitance = 1.0 - (density * rcpThickness);
				density_along_cone += (density * transmitance);
			}
		}

		p += light_step;
		coneR += 1.0/6.0;
	}

	pos += light_step * 8.0;
	weatherData = texture(weather, getWeatherTexCoordForPlane(pos));
	float hf = getHeightFraction(pos);
	if(hf <= 1.0)
	{
		float tempDensity = sampleCloudDensity(p, texture(weather, getWeatherTexCoordForPlane(p)).rgb);
		if(tempDensity > 0.0)
		{
			density += tempDensity;
			float transmitance = 1.0 - (density * rcpThickness);
			density_along_cone += (density * transmitance);
		}
	}

	float pwd = Powder(density_along_cone, ca);
	float beer = Beer(originalDensity);
	float hg1 = HenyeyGreenstein(d, l, 0.8, ca);
	float hg2 = HenyeyGreenstein(d, l, -0.5, ca);
	float lhg = mix(hg1, hg2, 0.5);

	return 2.0 * pwd + beer + lhg;
}*/

float raymarchCloud(vec3 p, vec3 endPos, vec3 d)
{
	vec3 dir = endPos - p;

	float thickNess = length(dir);

	int sample_count = int(ceil(mix(64, 128, clamp((thickNess - 10.0) / 10.0, 0.0, 1.0) )));
	float stepF = thickNess / float(sample_count);
	dir /= thickNess;

	float density = 0.0;
	float cloud_test = 0.0;
	int zero_density_sample_count = 0;

	float result = 0.0;

	for(int i = 0; i < sample_count; i++)
	{
		/*
		if (cloud_test > 0.0)
		{
			vec3 weather_data = texture(weather, getWeatherTexCoordForPlane(p)).rgb;

			float sampled_density = sampleCloudDensity(p, weather_data);
			
			if( sampled_density == 0.0)
			{
				zero_density_sample_count++;
			}
			
			if(zero_density_sample_count != 6)
			{
				density += sampled_density;
				
				float tempR = 1.0 * density;
				result = (1.0 - density) * tempR + result;

				if(density >= 1.0)
					return result;
		
				p += dir * stepF;
			} 
			else
			{
				cloud_test = 0.0;
				zero_density_sample_count = 0;
			}
		}
		else
		{
			vec3 weather_data = texture(weather, getWeatherTexCoordForPlane(p)).rgb;
			cloud_test = sampleCloudDensity(p, weather_data);
			if(cloud_test == 0.0)
			{
				p += dir * stepF;
			}
		}
		*/

		vec3 weatherData = texture(weather, getWeatherTexCoordForPlane(p)).rgb;
		float cloudDensity = sampleCloudDensity(p, weatherData);

		if(cloudDensity > 0.0)
		{
			density += cloudDensity;
			float transmittance = 1.0f - (density / thickNess);
			float tempR = 1.0 * cloudDensity * transmittance;
			result = (1.0 - density) * tempR + result;

			if(density >= 1.0)
				return result;
		}	
		p += dir * stepF;
	}

	//vec3 weather_data = texture(weather, getWeatherTexCoordForPlane(p)).rgb;

	return result;
}

vec3 intersectAtmosphere(vec3 camWorldPos, vec3 worldDir, float radius)
{
	vec3 p = camWorldPos - sphereCenter;
	float b = dot( p, worldDir );
	float c = dot( p, p ) - (radius * radius); // minus because we are inside the sphere
	
	float f = b * b - c;
	if(f >= 0.0) {
		float t1 = -b - sqrt( f );
		float t2 = -b + sqrt( f );

		float max = t1 > t2? t1 : t2;

		return camWorldPos + worldDir * max;
	}
	return vec3(0);
}

vec3 intersectPlane(vec3 camWorldPos, vec3 worldDir, float planeHeight)
{
	vec3 planeMin3 = vec3(0, planeHeight, 0);
	vec3 planeN = vec3(0, 1, 0);
	vec3 p0l0 = (planeMin3 - camWorldPos);
	float f = dot(p0l0, planeN);
	float d = dot(worldDir, planeN);

	float t = f / d;

	vec3 p = camWorldPos + worldDir * t;
	
	if(p.x < planeMin.x || p.x > planeMax.x || p.z < planeMin.y || p.z > planeMax.y)
		return vec3(0);

	return p;
}

void main()
{
	vec3 camWorldPos = (invView * vec4(0,0,0,1)).xyz;
	vec2 fulluv = texCoord * 2.0 - 1.0;
	float z = 1.0 / tan(radians(45.0));
	vec3 viewDir = normalize(vec3(fulluv, -z));

	vec3 worldDir = normalize( (invView * vec4(viewDir, 0)).xyz);
	vec3 pos = intersectPlane(camWorldPos, worldDir, 20.0);//intersectAtmosphere(camWorldPos, worldDir);
	vec3 endPos = intersectPlane(camWorldPos, worldDir, 30.0);

	//vec3 pos = intersectAtmosphere(camWorldPos, worldDir, 60.0);
	//vec3 endPos = intersectAtmosphere(camWorldPos, worldDir, 80.0);
	
	if(length(pos) > 0) // only draw if we hit a point above the horizon line
	{
		float density = raymarchCloud(pos, endPos, worldDir);

		if(density < 0.01)
			discard;

		//float density = texture(weather, getWeatherTexCoordForPlane(pos)).r * 2.0;

		//if(density < 0.1)
		//	discard;
		density = 1.0 - clamp(density, 0.0, 1.0);
		color = vec4(density, density, density, 1.0);
		emission = vec4(0,0,0,0);

		vec4 projPos = viewProj * vec4(pos, 1);
		projPos /= projPos.w;
		gl_FragDepth = projPos.z;
	}
	else
	{
		discard;
	}

	
	//if(length(pos) > 0)
	//{
	//	color = vec4(1, 0, 0, 1);
	//	emission = vec4(0,0,0,0);
		
	//	vec4 projPos = viewProj * vec4(pos, 1);
	//	projPos /= projPos.w;
	//	gl_FragDepth = projPos.z;
	//}
	
}