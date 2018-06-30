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
	 * Class in charge to manage the compute shaders that fill the cloud's 3d volume textures
	 */
	class VolumeTextureProgram : public ComputeProgram
	{
	private:
		// Texture output shader location
		unsigned int uOutput;
	public:
		VolumeTextureProgram(std::string shaderFile);
		VolumeTextureProgram(const VolumeTextureProgram & other);

		void configureProgram();
		void bindOutput(const TextureInstance * ti);
	};
}