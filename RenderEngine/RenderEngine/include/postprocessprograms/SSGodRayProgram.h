/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manage the screen space light scattering post process program
	 */
	class SSGodRayProgram : public PostProcessProgram
	{
	public:
		// Program unique name
		static const std::string PROGRAM_NAME;
	private:
		// Screen space light position id
		unsigned int uLightScreenPos;

		// Rays weight id
		unsigned int uWeight;
		// Rays density id
		unsigned int uDensity;
		// Rays intensity decay id
		unsigned int uDecay;
		// Exposure id
		unsigned int uExposure;
		// "Dont do anything" flag id
		unsigned int uOnlyPass;

		unsigned int uAlpha;
	public:
		SSGodRayProgram(std::string name, unsigned long long params);
		SSGodRayProgram(const SSGodRayProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// ============================================================
	// Creates new light scattering programs
	class SSGodRayProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}