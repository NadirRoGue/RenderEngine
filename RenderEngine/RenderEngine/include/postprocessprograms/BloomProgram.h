#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class BloomProgram : public PostProcessProgram
	{
	public:
		const static std::string PROGRAM_NAME;
	public:
		BloomProgram(std::string name, unsigned long long parameters);
		BloomProgram(const BloomProgram & other);
	};

	// ============================================================

	class BloomProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}