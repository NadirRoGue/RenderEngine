#pragma once

#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>

#include "ProceduralVegetation.h"
#include <vector>
#include <random>

namespace Engine
{
	class FractalTree : public ProceduralVegetation
	{
	private:
		std::vector<glm::vec3> vertices;
		std::vector<glm::ivec3> faces;
		std::vector<glm::vec3> colors;
		std::vector<glm::vec3> emission;
		Mesh * base;

		std::uniform_real_distribution<float> randGen;
		std::default_random_engine randEngine;

	public:
		FractalTree(const TreeGenerationData & data);
		Mesh * generate();
	private:
		void processChunk(glm::mat4 & mat, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotation, unsigned int depth = 0);
		void addLeaf(glm::mat4 & mat, glm::vec3 lastScaling);
		void appendVerticesAndFaces(glm::mat4 & model, glm::vec3 scale, unsigned int depth, bool isLeaf = false);

		float randSign();
		float randInInterval(float a, float b);
		glm::vec3 lerpColor(const glm::vec3 & a, const glm::vec3 & b, float alpha);
	};
}