#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class GaussianBlurProgram : public PostProcessProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uAffetedTexels;
		unsigned int uMaskSize;
		unsigned int uMask;
		unsigned int uTexelSize;

		unsigned int maskSize;
		float * kernel;
		float * affectedTexels;

	public:
		GaussianBlurProgram(std::string name, unsigned long long params);
		GaussianBlurProgram(const GaussianBlurProgram & other);
		~GaussianBlurProgram();

		virtual void configureProgram();
		virtual void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		void setMaskSize(unsigned int ms);
		void setKernel(float * k);
		void setAffectedTexels(glm::vec2 * at);
	};

	class GaussianBlurProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}