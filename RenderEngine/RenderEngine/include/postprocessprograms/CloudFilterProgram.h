/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class CloudFilterProgram : public PostProcessProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uTexelSize;

		unsigned int uGColor;
	public:
		CloudFilterProgram(std::string name, unsigned long long params);
		CloudFilterProgram(const CloudFilterProgram & other);
		~CloudFilterProgram();

		virtual void configureProgram();
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);
		
		void setBufferInput(const TextureInstance * color);
	};

	class CloudFilterProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}