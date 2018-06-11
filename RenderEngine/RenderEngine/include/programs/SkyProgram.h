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

		unsigned int uSkyZenitColor;
		unsigned int uSkyHorizonColor;

		unsigned int inPos;

	public:
		SkyProgram(std::string name, unsigned long long params);
		SkyProgram(const SkyProgram & other);

		void configureProgram();
		void configureMeshBuffers(Mesh * mesh);
		void onRenderObject(const Object * obj, Camera * camera);
	};

	class SkyProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}