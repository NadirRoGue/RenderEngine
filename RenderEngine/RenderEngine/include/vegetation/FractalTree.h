/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#define GLM_FORCE_RADIANS

#include <glm\glm.hpp>

#include "ProceduralVegetation.h"
#include <vector>
#include <random>

namespace Engine
{
	/**
	 * Procedural vegetation generator using a fractal algorithm
	 */
	class FractalTree : public ProceduralVegetation
	{
	private:
		// Vertices of the final generated tree
		std::vector<glm::vec3> vertices;
		// Faces of the final generated tree
		std::vector<glm::ivec3> faces;
		// Texture coordinates of the final generated tree
		std::vector<glm::vec2> uvs;
		// Per vertex color of the final generated tree
		std::vector<glm::vec3> colors;
		// Per vertex "emission" (actually is used to carry extra info) of the final generated tree
		std::vector<glm::vec3> emission;
		
		// Cube base shape to build the tree
		Mesh * base;
		Mesh * leaf;

		// Random number generator
		std::uniform_real_distribution<float> randGen;
		std::default_random_engine randEngine;

	public:
		FractalTree(const TreeGenerationData & data);
		Mesh * generate();
	private:
		// Process a chunk of the tree, adding base shape data according to the growth and stopping at the appropiate depth
		void processChunk(glm::mat4 mat, glm::vec3 scale, glm::vec3 translate, glm::vec3 rotation, size_t vOffset, unsigned int depth);
		// Adds leafs to the final tree data
		void addLeaf(glm::mat4 mat, glm::vec3 lastScaling, size_t offset, unsigned int depth);
		// Utility function used to fill the class vectors
		void appendVerticesAndFaces(Mesh * source, glm::mat4 model, glm::vec3 scale, unsigned int depth, size_t vOffset, bool keepBase, bool isLeaf = false);

		// Returns a random sign (either positive or negative)
		float randSign();
		// Returns a random number within the given interval
		float randInInterval(float a, float b);
	};
}