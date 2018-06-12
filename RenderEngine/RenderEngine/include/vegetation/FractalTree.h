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
		//std::vector<glm::vec2> uvs;
		Mesh * base;
		Mesh * leaf;

		std::uniform_real_distribution<float> randGen;
		std::default_random_engine randEngine;

	public:
		FractalTree(const TreeGenerationData & data);
		Mesh * generate();
	private:
		void processChunk(glm::mat4 & mat, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotation, size_t vOffset, unsigned int depth);
		void addLeaf(glm::mat4 & mat, glm::vec3 lastScaling, size_t offset, unsigned int depth);
		void appendVerticesAndFaces(Mesh * source, glm::mat4 & model, glm::vec3 scale, unsigned int depth, size_t vOffset, bool keepBase, bool isLeaf = false);

		float randSign();
		float randInInterval(float a, float b);
		glm::vec3 lerpColor(const glm::vec3 & a, const glm::vec3 & b, float alpha);
	};
}