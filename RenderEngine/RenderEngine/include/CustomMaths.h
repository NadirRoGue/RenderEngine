/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Engine
{
	glm::vec3 voronoiTriangleAreas(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C);

	float cotangent(const glm::vec3 &pivot, const glm::vec3 &a, const glm::vec3 &b);

	glm::vec3 tangent(const glm::vec2 &st1, const glm::vec2 st2, const glm::vec3 &Q1, const glm::vec3 &Q2);
}