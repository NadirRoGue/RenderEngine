#pragma once

#include "ComputeProgram.h"
#include "instances/TextureInstance.h"

namespace Engine
{
	class VolumeTextureProgram : public ComputeProgram
	{
	private:
		unsigned int uOutput;
	public:
		VolumeTextureProgram(std::string shaderFile);
		VolumeTextureProgram(const VolumeTextureProgram & other);

		void configureProgram();
		void bindOutput(const TextureInstance * ti);
	};
}