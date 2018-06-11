#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class SSAAProgram : public PostProcessProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uTexelSize;
	public:
		SSAAProgram(std::string name, unsigned long long params);
		SSAAProgram(const SSAAProgram & other);
		~SSAAProgram();

		virtual void configureProgram();
		virtual void onRenderObject(const Object * obj, Camera * camera);
	};

	class SSAAProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}