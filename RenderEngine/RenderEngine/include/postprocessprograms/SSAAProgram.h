/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	/**
	 * Class in charge to manage the screen space anti aliasing post process program
	 */
	class SSAAProgram : public PostProcessProgram
	{
	public:
		// Program unique name
		static std::string PROGRAM_NAME;
	private:
		// Screen space texel size id
		unsigned int uTexelSize;
	public:
		SSAAProgram(std::string name, unsigned long long params);
		SSAAProgram(const SSAAProgram & other);
		~SSAAProgram();

		virtual void configureProgram();
		virtual void onRenderObject(const Object * obj, Camera * camera);
	};

	// ===============================================================
	// Screen Space anti aliasing factory
	class SSAAProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}