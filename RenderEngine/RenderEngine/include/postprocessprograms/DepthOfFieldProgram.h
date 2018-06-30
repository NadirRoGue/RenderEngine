/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manage the depth of field post process program
	 */
	class DepthOfFieldProgram : public PostProcessProgram
	{
	public:
		// Program unique name
		static std::string PROGRAM_NAME;
	private:
		// Focal distance id
		unsigned int uFocalDistance;
		// Max distance id
		unsigned int uMaxDistanceFactor;
		// Inverse projection matrix id
		unsigned int uInverseProj;
		// Texel screen size id
		unsigned int uTexelSize;
		// G-Buffer depth texture id
		unsigned int uDepthBuffer;
	public:
		DepthOfFieldProgram(std::string name, unsigned long long params);
		DepthOfFieldProgram(const DepthOfFieldProgram & other);
		~DepthOfFieldProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// =========================================================================
	// Depth of field factory: creates new depth of field programs
	class DepthOfFieldProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}