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

		unsigned int uPerlinWorley;
		unsigned int uWorley;
		unsigned int uWeather;

		unsigned int uResolution;
		unsigned int uFov;

		unsigned int uCamPos;
		unsigned int uLightDir;
		unsigned int uLightColor;
		unsigned int uZenitColor;
		unsigned int uHorizonColor;
		unsigned int uCloudColor;
		unsigned int uTime;
		unsigned int uCloudType;
		unsigned int uCloudSpeed;
		unsigned int uCoverageMultiplier;

		unsigned int uCurrentDepth;

		unsigned int uFrame;
		unsigned int f;
	public:
		VolumetricCloudProgram(std::string name, unsigned long long params);
		VolumetricCloudProgram(const VolumetricCloudProgram & other);

		void configureProgram();
		void onRenderObject(Object * obj, Camera * camera);
	};

	class VolumetricCloudProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}