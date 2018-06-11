#pragma once

#include "PostProcessProgram.h"

namespace Engine
{
	class DepthOfFieldProgram : public PostProcessProgram
	{
	public:
		static std::string PROGRAM_NAME;
	private:
		unsigned int uFocalDistance;
		unsigned int uMaxDistanceFactor;
		unsigned int uInverseProj;
		unsigned int uTexelSize;

		float focalDistance;
		float maxDistanceFactor;

	public:
		DepthOfFieldProgram(std::string name, unsigned long long params);
		DepthOfFieldProgram(const DepthOfFieldProgram & other);
		~DepthOfFieldProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, Camera * camera);

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