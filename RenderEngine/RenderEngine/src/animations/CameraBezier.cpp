#include "animations/CameraBezier.h"

#include "WorldConfig.h"
#include "Time.h"

#include <iostream>

Engine::CameraBezier::CameraBezier(Engine::Camera * camera, glm::vec3 & centerOfSpline, float splineRadius, float moveSpeed)
	:Engine::Animation("CameraBezier", NULL),cam(camera),moveSpeed(moveSpeed),splineRadius(splineRadius),currentIndex(0),alpha(0.0f)
{
	alphaStep = 1.0f / (splineRadius);
	this->splineCenter = -centerOfSpline;
	computeSpline();
	previousPoint = splinePoints[0];
	paused = true;

	//cam->rotateView(glm::vec3(0, 3.1415f, 0));
}

void Engine::CameraBezier::update()
{
	unsigned int tm = Engine::Settings::travelMethod;
	Engine::TravelMethod tmEnum = static_cast<Engine::TravelMethod>(tm);
	if (tmEnum == Engine::TravelMethod::TRAVEL_BEZIER)
	{
		glm::vec3 nextPoint = evaluateCurrentSpline();
		if (glm::length(previousPoint - nextPoint) > 0)
		{
			cam->setLookAt(-previousPoint, -nextPoint);
		}
		previousPoint = nextPoint;
	}
}

void Engine::CameraBezier::computeSpline()
{
	// this little algorithm has been developed playing around with
	// https://www.desmos.com/calculator/cahqdxeshd
	
	std::vector<glm::vec3> points;
	createSquare(points);

	size_t end = points.size() - 1;
	for (size_t i = 0; i < end; i++)
	{
		// Get current pair of points to process
		glm::vec3 p0 = points[i];
		glm::vec3 p1 = points[i + 1];

		//glm::vec3 tempP0 = glm::vec3(p0.x, 0, p0.z);
		//glm::vec3 tempP1 = glm::vec3(p1.x,)

		// Create the direction vector (from one point to another)
		// Create the normal vector (normal to the "face" formed by the 2 points)
		// We will use them to create the control points for this pair of points
		glm::vec3 dirVector = p1 - p0;

		float len = (glm::length(dirVector) / 3.0f);
		len *= 0.75;
		
		// Get the operands needed to build the control points from the points themselves
		dirVector = glm::normalize(dirVector) * len;
		glm::vec3 normalVector = glm::normalize(((p0 - splineCenter) + (p1 - splineCenter)) / 2.0f) * len;
		
		// create control point 0
		glm::vec3 cp0 = p0 + dirVector + normalVector;

		// create control point 1
		glm::vec3 cp1 = p1 - dirVector + normalVector;

		// add the points to the spline
		splinePoints.push_back(p0);
		splinePoints.push_back(cp0);
		splinePoints.push_back(cp1);
	}

	splinePoints.push_back(splinePoints[0]);
}

void Engine::CameraBezier::createSquare(std::vector<glm::vec3> & result)
{
	result.reserve(5);

	glm::vec3 a = splineCenter + glm::vec3(-splineRadius, 0, splineRadius);
	glm::vec3 b = splineCenter + glm::vec3(splineRadius, 0, splineRadius);
	glm::vec3 c = splineCenter + glm::vec3(splineRadius, 0, -splineRadius);
	glm::vec3 d = splineCenter + glm::vec3(-splineRadius, 0, -splineRadius);

	result.push_back(a);
	result.push_back(b);
	result.push_back(c);
	result.push_back(d);
	result.push_back(a); // re-add first to create a closed circle
}

glm::vec3 Engine::CameraBezier::evaluateCurrentSpline()
{
	// Get current points to evaluate (2 points and 2 control points)
	glm::vec3 p1 = splinePoints[currentIndex];		// point
	glm::vec3 p2 = splinePoints[currentIndex + 1];	// control point
	glm::vec3 p3 = splinePoints[currentIndex + 2];	// control point
	glm::vec3 p4 = splinePoints[currentIndex + 3];	// point

	// precompute 
	const float oneMinusAlpha = 1 - alpha;

	// evaluate bezier cubic spline
	glm::vec3 result =
		((oneMinusAlpha * oneMinusAlpha * oneMinusAlpha)*p1)
		+ (3 * (oneMinusAlpha*oneMinusAlpha)*alpha*p2)
		+ (3 * (oneMinusAlpha)*alpha*alpha*p3)
		+ (alpha*alpha*alpha*p4);

	//Adjust alpha/point index
	if (alpha >= 1.0f)
	{
		alpha = 0.0f;
		currentIndex += 3;
		currentIndex %= (splinePoints.size() - 1);
	}
	else
	{
		alpha += alphaStep * Engine::Time::deltaTime * moveSpeed;
		alpha = alpha > 1.0f ? 1.0f : alpha;
	}

	return result;
}
