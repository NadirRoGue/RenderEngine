/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manage the volumetric clouds programs
	 */
	class VolumetricCloudProgram : public PostProcessProgram
	{
	public:
		// Unique program name
		static const std::string PROGRAM_NAME;
	private:
		// Inverse view matrix (transforms raymarch rays)
		unsigned int uInvView;

		// Perlin-Worley 3D noise texture id (cloud shape)
		unsigned int uPerlinWorley;
		// Worley 3D noise texture id (cloud details)
		unsigned int uWorley;
		// Weather info texture id
		unsigned int uWeather;

		// Screen resolution id
		unsigned int uResolution;
		// Camera field of view id
		unsigned int uFov;

		// Camera position id
		unsigned int uCamPos;
		// Light direction id
		unsigned int uLightDir;
		// Light color id
		unsigned int uLightColor;
		// Zenit color id (fog / ambient)
		unsigned int uZenitColor;
		// Horizon color id (fog / ambient)
		unsigned int uHorizonColor;
		// Cloud color multiplier id
		unsigned int uCloudColor;
		// Time passed id
		unsigned int uTime;
		// Cloud type id
		unsigned int uCloudType;
		// Cloud speed id
		unsigned int uCloudSpeed;
		// Coverage multiplier id
		unsigned int uCoverageMultiplier;

		// Depth texture info id
		unsigned int uCurrentDepth;

		// Current frame id
		unsigned int uFrame;
		// Current frame
		unsigned int f;
	public:
		VolumetricCloudProgram(std::string name, unsigned long long params);
		VolumetricCloudProgram(const VolumetricCloudProgram & other);

		void configureProgram();
		void onRenderObject(Object * obj, Camera * camera);
	};

	// ==========================================================================
	// Creates new volumetric clouds programs
	class VolumetricCloudProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}