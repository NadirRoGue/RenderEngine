/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "ComputeProgram.h"
#include "instances/TextureInstance.h"

namespace Engine
{
	/**
	 * Class in charge to manage the compute shader that fills the weather texture for the volumetric clouds
	 */
	class WeatherTextureProgram : public ComputeProgram
	{
	private:
		// Texture output location in shader
		unsigned int uWeatherTex;

	public:
		WeatherTextureProgram();
		WeatherTextureProgram(const WeatherTextureProgram & other);

		void configureProgram();
		void bindOutput(const TextureInstance * ti);
	};
}