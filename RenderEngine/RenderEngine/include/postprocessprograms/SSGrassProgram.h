#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class SSGrassProgram : public PostProcessProgram
	{
	public:
		static const std::string PROGRAM_NAME;
	private:
		unsigned int uScreenSize;
		unsigned int uGrassInfoBuffer;
		unsigned int uPosBuffer;
	public:
		SSGrassProgram(std::string name, unsigned long long params);
		SSGrassProgram(const SSGrassProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 & proj);
	};

	// =======================================================================

	class SSGrassProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}