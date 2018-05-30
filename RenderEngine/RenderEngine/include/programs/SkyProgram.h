#pragma once

#include "Program.h"

namespace Engine
{
	class SkyProgram : public Program
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uProjMatrix;
		unsigned int uLightDir;
		unsigned int uLightColor;

		unsigned int inPos;

	public:
		SkyProgram(std::string name, unsigned long long params);
		SkyProgram(const SkyProgram & other);

		void configureProgram();
		void configureMeshBuffers(Mesh * mesh);
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
	};

	class SkyProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}