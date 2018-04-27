#pragma once

#include "programs/ProceduralTerrainProgram.h"

namespace Engine
{
	class ProceduralWaterProgram : public ProceduralTerrainProgram
	{
	public:
		static std::string PROGRAM_NAME;
	public:
		ProceduralWaterProgram(std::string name, unsigned long long parameters);
		ProceduralWaterProgram(const ProceduralWaterProgram & other);
	};

	// =========================================================

	class ProceduralWaterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}