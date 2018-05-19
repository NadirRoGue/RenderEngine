#pragma once

#include "ComputeProgram.h"
#include "instances/TextureInstance.h"

namespace Engine
{
	class WeatherTextureProgram : public ComputeProgram
	{
	private:
		unsigned int uWeatherTex;

	public:
		WeatherTextureProgram();
		WeatherTextureProgram(const WeatherTextureProgram & other);

		void configureProgram();
		void bindOutput(const TextureInstance * ti);
	};
}