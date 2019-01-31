/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manipulate the image space filter applied to volumetric clouds
	 */
	class CloudFilterProgram : public PostProcessProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		// texel size (1/bufferwidth, 1/bufferheight)
		unsigned int uTexelSize;

		// Light color id (will determine the ray's color)
		unsigned int uLightColor;

		// Alternate rendered frames for temporal reprojection
		unsigned int uRepro1;
		unsigned int uRepro2;
		unsigned int uRepro3;
		unsigned int uRepro4;

		unsigned int uVel1, uVel2, uVel3, uVel4;

		unsigned int uFrameIter;
		unsigned int uOldProjView;

		glm::mat4 oldView;

	public:
		CloudFilterProgram(std::string name, unsigned long long params);
		CloudFilterProgram(const CloudFilterProgram & other);
		~CloudFilterProgram();

		virtual void configureProgram();
		virtual void onRenderObject(const Object * obj, Camera * camera);
		
		void setBufferInput(TextureInstance ** buffers);
		void setVelocityInput(TextureInstance ** velocities);
	};

	// ========================================================================
	// Cloud filter factory : creates new cloud filter programs
	class CloudFilterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}