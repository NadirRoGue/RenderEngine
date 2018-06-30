/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manage the HDR Tone mapping post process program
	 */
	class HDRToneMappingProgram : public PostProcessProgram
	{
	public:
		// Program unique name
		static const std::string PROGRAM_NAME;
	private:
		// Exposure id
		unsigned int uExposure;
		// Gamma correction id
		unsigned int uGamma;
		// Tint to apply id
		unsigned int uTint;
	public:
		HDRToneMappingProgram(std::string name, unsigned long long params);
		HDRToneMappingProgram(const HDRToneMappingProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// ====================================================================
	// HDR Tone mapping factory: creates new tone mapping programs
	class HDRToneMappingProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}