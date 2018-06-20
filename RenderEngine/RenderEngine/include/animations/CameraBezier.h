/**
 * @author Nadir Román Guerrero
 * @email nadir.ro.gue@gmail.com
 */

#pragma once

#include "Animation.h"
#include "Camera.h"

#include <vector>

namespace Engine
{
	/**
	 * Animation class in charge of automatically animate the camera
	 * following a Bezier spline
	 */
	class CameraBezier : public Animation
	{
	private:
		// Camera to animate
		Camera * cam;

		// Advance speed
		float moveSpeed;
		// Current evaluation node
		size_t currentIndex;

		// Interpolation parameters
		float alpha;
		float alphaStep;

		// Spline data
		glm::vec3 splineCenter;
		float splineRadius;
		std::vector<glm::vec3> splinePoints;
		glm::vec3 previousPoint;
	public:
		CameraBezier(Camera * cam, glm::vec3 centerOfSpline, float splineRadius, float moveSpeed);
		void update();
	private:
		// Generates a square around the rotation point
		void createSquare(std::vector<glm::vec3> & result);
		// Computes the spline
		void computeSpline();
		// Evaluate the spline at the current position
		glm::vec3 evaluateCurrentSpline();
	};
}