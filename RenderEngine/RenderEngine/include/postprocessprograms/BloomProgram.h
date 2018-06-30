/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "postprocessprograms/SSAAProgram.h"

#include "DeferredRenderObject.h"

namespace Engine
{
	/**
	 * Class in charge to manipulate the Bloom post-process
	 */ 
	class BloomProgram : public SSAAProgram
	{
	public:
		// Class unique identifier
		const static std::string PROGRAM_NAME;
	private:
		// Structure used to hold the different RTTs the post-process has to 
		// switch to when bluring the emissive texture
		typedef struct SwitchableBuffers
		{
			DeferredRenderObject * pass;
			TextureInstance * color;
			TextureInstance * emissive;
		} SwitchableBuffers;
	private:
		// Switchable RTTs
		SwitchableBuffers buf[2];
		// Num of passes
		unsigned int passes;

		// Shader flag to tell whether we should blur horizontally or vertically
		unsigned int uHorizontal;
		// Shader flags to tell wether its the last pass and we should blend the result
		// with the base color output
		unsigned int uBlend;
	public:
		BloomProgram(std::string name, unsigned long long parameters);
		BloomProgram(const BloomProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// ============================================================

	// Bloom Factory: creates new bloom programs
	class BloomProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}