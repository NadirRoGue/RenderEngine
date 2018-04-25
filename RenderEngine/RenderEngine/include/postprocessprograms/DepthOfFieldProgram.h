#pragma once

#include "postprocessprograms/GaussianBlurProgram.h"

namespace Engine
{
	class DepthOfFieldProgram : public GaussianBlurProgram
	{
	private:
		unsigned int uFocalDistance;
		unsigned int uMaxDistanceFactor;
		unsigned int uInverseProj;

		float focalDistance;
		float maxDistanceFactor;

	public:
		DepthOfFieldProgram(std::string name);
		DepthOfFieldProgram(const DepthOfFieldProgram & other);
		~DepthOfFieldProgram();

		void configureProgram();
		void onRenderObject(const Object * obj, const glm::mat4 & view, const glm::mat4 &proj);

		float getFocalDistance();
		float getMaxDistanceFactor();

		void setFocalDistance(float fd);
		void setMaxDistanceFactor(float mdf);
	};
}