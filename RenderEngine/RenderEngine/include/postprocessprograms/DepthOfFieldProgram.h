#pragma once

#include "postprocessprograms/GaussianBlurProgram.h"

namespace Engine
{
	class DepthOfFieldProgram : public GaussianBlurProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uFocalDistance;
		unsigned int uMaxDistanceFactor;
		unsigned int uInverseProj;

		float focalDistance;
		float maxDistanceFactor;

	public:
		DepthOfFieldProgram(std::string name, unsigned long long params);
		DepthOfFieldProgram(const DepthOfFieldProgram & other);
		~DepthOfFieldProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		float getFocalDistance();
		float getMaxDistanceFactor();

		void setFocalDistance(float fd);
		void setMaxDistanceFactor(float mdf);
	};

	class DepthOfFieldProgramFactory : public ProgramFactory
	{
	protected:
		Program * createProgram(unsigned long long parameters);
	};
}