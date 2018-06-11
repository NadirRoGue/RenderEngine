#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class SSReflectionProgram : public PostProcessProgram
	{
	public:
		static const std::string PROGRAM_NAME;
	private:
		unsigned int uProjMat;
		unsigned int uPosBuffer;
		unsigned int uNormalBuffer;
		unsigned int uDepthBuffer;
		unsigned int uSpecularBuffer;
		unsigned int uLightDir;
	public:
		SSReflectionProgram(std::string name, unsigned long long params);
		SSReflectionProgram(const SSReflectionProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// =====================================================================

	class SSReflectionProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}