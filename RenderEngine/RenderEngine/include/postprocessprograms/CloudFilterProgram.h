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

		// Alternate rendered frames for temporal reprojection
		// even frame will contain all pixels with an even x coordinate
		unsigned int uEvenFrame;
		// odd frame will contain all pixels with an odd x coordinate
		unsigned int uOddFrame;
		// Current frame to process
		unsigned int uFrame;
	public:
		CloudFilterProgram(std::string name, unsigned long long params);
		CloudFilterProgram(const CloudFilterProgram & other);
		~CloudFilterProgram();

		virtual void configureProgram();
		virtual void onRenderObject(const Object * obj, Camera * camera);
		
		void setBufferInput(const TextureInstance * evenFrame, const TextureInstance * oddFrame);
	};

	// ========================================================================
	// Cloud filter factory : creates new cloud filter programs
	class CloudFilterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}