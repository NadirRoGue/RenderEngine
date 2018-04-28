#pragma once

#include "programs/ProceduralTerrainProgram.h"

namespace Engine
{
	class ProceduralWaterProgram : public ProceduralTerrainProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uTime;
	public:
		ProceduralWaterProgram(std::string name, unsigned long long parameters);
		ProceduralWaterProgram(const ProceduralWaterProgram & other);
		void configureProgram();
		void setTimeUniform(float sinTime);
	};

	// =========================================================

	class ProceduralWaterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}