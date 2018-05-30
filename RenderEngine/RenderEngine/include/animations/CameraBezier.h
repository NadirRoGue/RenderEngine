#pragma once

#include "Animation.h"
#include "Camera.h"

#include <vector>

namespace Engine
{
	class CameraBezier : public Animation
	{
	private:
		Camera * cam;

		float moveSpeed;
		size_t currentIndex;

		float alpha;
		float alphaStep;

		glm::vec3 splineCenter;
		float splineRadius;
		std::vector<glm::vec3> splinePoints;
		glm::vec3 previousPoint;
		bool paused;
	public:
		CameraBezier(Camera * cam, glm::vec3 & centerOfSpline, float splineRadius, float moveSpeed);
		void update();
	private:
		void createSquare(std::vector<glm::vec3> & result);
		void computeSpline();
		glm::vec3 evaluateCurrentSpline();
	};
}