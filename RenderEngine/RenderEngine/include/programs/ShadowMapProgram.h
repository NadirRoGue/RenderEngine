#include "Program.h"

namespace Engine
{
	class ShadowMapProgram : public Program
	{
	public:
		static const std::string PROGRAM_NAME;
	private:

	public:
	};

	// =============================================

	class ShadowMapProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}