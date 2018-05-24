#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class VolumetricCloudProgram : public PostProcessProgram
	{
	public:
		static const std::string PROGRAM_NAME;
	private:
		unsigned int uInvView;
		unsigned int uInvProj;
		unsigned int uViewProj;

		unsigned int uPerlinWorley;
		unsigned int uWorley;
		unsigned int uWeather;

		unsigned int uCamPos;
		unsigned int uLightDir;
		unsigned int uTime;
		unsigned int uCloudType;
		unsigned int uCloudSpeed;
		unsigned int uCoverageMultiplier;
	public:
		VolumetricCloudProgram(std::string name, unsigned long long params);
		VolumetricCloudProgram(const VolumetricCloudProgram & other);

		void configureProgram();
		void onRenderObject(Object * obj, const glm::mat4 & view, const glm::mat4 & proj);
	};

	class VolumetricCloudProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}