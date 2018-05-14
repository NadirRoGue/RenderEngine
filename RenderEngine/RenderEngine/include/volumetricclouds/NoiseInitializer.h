#pragma once

#include "postprocessprograms/VolumetricTextureProgram.h"

namespace Engine
{
	namespace CloudSystem
	{
		class NoiseInitializer
		{
		public:
			static const NoiseInitializer * INSTANCE;
		private:
			unsigned int uPWFbo;
			unsigned int uWFbo;
			unsigned int uCFbo;
			unsigned int uWFbo;

			// We could go for more res, but it looks fine like this

			// r = Perlin-Worley FBM X octave
			// g = Worley X + 1 octave
			// b = Worley X + 2 octave
			// c = Worley X + 3 octave
			// res 128 * 128 * 128
			TextureInstance * PerlinWorleyFBM;
			TextureInstance * PWFBMDepth;

			// r = Worley X octave
			// g = Worley X + 1 octave
			// b = Worley X + 2 octave
			// res 32 * 32 * 32
			TextureInstance * WorleyFBM;
			TextureInstance * WFBMDepth;

			// r = Horizontal Curl
			// g = Vertical Curl
			// b = Transversal Curl
			// res 128 * 128
			TextureInstance * CurlNoise;
			TextureInstance * CNDepth;

			// r = Cloud coverage (Clamped Perlin: 0 if perling < 0.7, perlin otherwise)
			// g = Rain
			// b = Cloud type
			// res 1024 * 1024 (maps to a big ass plane in the sky)
			TextureInstance * WeatherData;
			TextureInstance * WDDepth;

			// Shader to write to textures from GPU (CPU generation would be slow)
			VolumetricTextureProgram * generator;
		public:
			const TextureInstance * getPerlinWorleyFBM() const;
			const TextureInstance * getWorleyFBM() const;
			const TextureInstance * getCurlNoise() const;
			const TextureInstance * getWeatherData() const;

			void render() const;
		private:
			NoiseInitializer();
			void init();
			void initShader();
			void initTextures();
			void initializeFBO();

			TextureInstance * createDepthTexture(unsigned int w, unsigned int h);

			void clean();
		};
	}
}