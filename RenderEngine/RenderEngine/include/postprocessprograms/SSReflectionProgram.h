/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manage the screen space reflection post process program
	 */
	class SSReflectionProgram : public PostProcessProgram
	{
	public:
		// Program unique name
		static const std::string PROGRAM_NAME;
	private:
		// Projection matrix id
		unsigned int uProjMat;
		// Position texture buffer id
		unsigned int uPosBuffer;
		// Normal texture buffer id
		unsigned int uNormalBuffer;
		// Depth texture buffer id
		unsigned int uDepthBuffer;
		// Reflection texture buffer id
		unsigned int uSpecularBuffer;
		// Light direction
		unsigned int uLightDir;
	public:
		SSReflectionProgram(std::string name, unsigned long long params);
		SSReflectionProgram(const SSReflectionProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// =====================================================================
	// Creates new screen space reflection programs
	class SSReflectionProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}