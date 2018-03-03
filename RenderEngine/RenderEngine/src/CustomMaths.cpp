/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "CustomMaths.h"

glm::vec3 Engine::voronoiTriangleAreas(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C)
{
	float aArea, bArea, cArea;

	if (glm::dot(B - A, C - A) < 0)
	{
		float triangleArea = glm::cross(B - A, C - A).length() * 0.5f;
		aArea = 0.5f * triangleArea;
		bArea = cArea = 0.25f * triangleArea;
	}
	else if (glm::dot(A - B, C - B) < 0)
	{
		float triangleArea = glm::cross(A - B, C - B).length() * 0.5f;
		bArea = 0.5f * triangleArea;
		aArea = cArea = 0.25f * triangleArea;
	}
	else if (glm::dot(A - C, B - C) < 0)
	{
		float triangleArea = glm::cross(A - C, B - C).length() * 0.5f;
		cArea = 0.5f * triangleArea;
		aArea = bArea = 0.25f * triangleArea;
	}
	else
	{
		glm::length_t AtoB = (B - A).length();
		glm::length_t AtoC = (C - A).length();
		glm::length_t BtoC = (C - B).length();

		float ctngA = cotangent(A, B, C);
		float ctngB = cotangent(B, A, C);
		float ctngC = cotangent(C, A, B);

		aArea = 0.125f * ((AtoB*AtoB*ctngC) + (AtoC*AtoC*ctngB));
		bArea = 0.125f * ((AtoB*AtoB*ctngC) + (BtoC*BtoC*ctngA));
		cArea = 0.125f * ((AtoC*AtoC*ctngB) + (BtoC*BtoC*ctngA));
	}

	return glm::vec3(aArea, bArea, cArea);
}

float Engine::cotangent(const glm::vec3 &pivot, const glm::vec3 &a, const glm::vec3 &b)
{
	glm::vec3 pa = a - pivot;
	glm::vec3 pb = b - pivot;

	pa = glm::normalize(pa);
	pb = glm::normalize(pb);

	const glm::length_t sinA = glm::cross(pa, pb).length();
	const float cosA = glm::dot(pa, pb);

	return (cosA / sinA);
}

glm::vec3 Engine::tangent(const glm::vec2 &st1, const glm::vec2 st2, const glm::vec3 &Q1, const glm::vec3 &Q2)
{
	const float stDet = 1.0f / ((st1.x*st2.y) - (st1.y*st2.x));

	glm::vec2 row1(st2.y, -st1.y);
	//glm::vec2 row2(-st2.x, st1.x);

	glm::vec2 column1(Q1.x, Q2.x);
	glm::vec2 column2(Q1.y, Q2.y);
	glm::vec2 column3(Q1.z, Q2.z);

	glm::vec3 tangent = stDet * glm::vec3(
		glm::dot(row1, column1),
		glm::dot(row1, column2),
		glm::dot(row1, column3)
	);

	return tangent;
}