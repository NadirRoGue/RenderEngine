#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class SSGodRayProgram : public PostProcessProgram
	{
	public:
		static const std::string PROGRAM_NAME;
	private:
		unsigned int uLightScreenPos;

		unsigned int uWeight;
		unsigned int uDensity;
		unsigned int uDecay;
		unsigned int uExposure;
		unsigned int uOnlyPass;
	public:
		SSGodRayProgram(std::string name, unsigned long long params);
		SSGodRayProgram(const SSGodRayProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 & proj);
	};

	// ============================================================

	class SSGodRayProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}