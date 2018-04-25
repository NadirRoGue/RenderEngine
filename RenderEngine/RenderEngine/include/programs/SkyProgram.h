#pragma once

#include "Program.h"
#include "instances/TextureInstance.h"

namespace Engine
{
	class SkyProgram : public Program
	{
	private:
		unsigned int uProjMatrix;
		unsigned int uCubeMap;

		unsigned int inPos;

	public:
		SkyProgram(std::string name);
		SkyProgram(const SkyProgram & other);

		void configureProgram();
		void configureMeshBuffers(MeshInstance * mesh);
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		void setCubemapUniform(TextureInstance * t);
	};
}