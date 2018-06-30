#pragma once

#include "Program.h"

namespace Engine
{
	class CloudShadowProgram : public Program
	{
	public:
		static const std::string PROGRAM_NAME;
	private:
		unsigned int uInPos;
		unsigned int uInUV;
		unsigned int uLightProjMat;
		unsigned int uWeather;
		unsigned int uCoverageMult;
	public:
		CloudShadowProgram(std::string name, unsigned long long params);
		CloudShadowProgram(const CloudShadowProgram & other);

		void configureMeshBuffers(Mesh * mesh);
		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
		void setUniformLightProjMatrix(const glm::mat4 & proj);
	};

	// =======================================================================

	class CloudShadowProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}