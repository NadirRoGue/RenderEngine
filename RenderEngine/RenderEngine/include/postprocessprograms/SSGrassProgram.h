/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manage the screen space grass post process program
	 */
	class SSGrassProgram : public PostProcessProgram
	{
	public:
		// Program unique name
		static const std::string PROGRAM_NAME;
	private:
		// Screen size id
		unsigned int uScreenSize;
		// Grass info texture id
		unsigned int uGrassInfoBuffer;
		// Fragment camera space position texture id
		unsigned int uPosBuffer;
	public:
		SSGrassProgram(std::string name, unsigned long long params);
		SSGrassProgram(const SSGrassProgram & other);

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);
	};

	// =======================================================================
	// Creates new screen space grass programs
	class SSGrassProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long params);
	};
}