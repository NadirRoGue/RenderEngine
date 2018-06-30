/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include "Mesh.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Engine
{
	// Struct to configure the generation of a tree. By filling this
	// structure and feeding it to any of the avaliable procedrual 
	// vegetation generator (currently, only the Fractal one), it will
	// create a tree based on this configuration
	typedef struct TreeGenerationData
	{
		std::string treeName;
		// Seed used in various parts of the generation where a random
		// number generator is need.
		unsigned long seed;

		// Max branch depth
		unsigned int maxDepth;
		// Depth at which start to create branches. Before that, only one branch
		// will be created to keep the tree growing
		unsigned int startBranchingDepth;
		// Max branches per depth level
		unsigned int maxBranchesSplit;

		// Wether to apply random rotation within the defined rotation intervals
		// to the first tree chunk (the main trunk)
		bool rotateMainTrunk;
		// Min and max rotation intervals (rotation is randomnly choosen from them)
		glm::vec3 minBranchRotation;
		glm::vec3 maxBranchRotation;

		// Scaling factor applied on every depth level
		glm::vec3 scalingFactor;
		
		// Trunk color
		glm::vec3 startTrunkColor;
		glm::vec3 endTrunkColor;

		// Leaf color
		bool emissiveLeaf;
		glm::vec3 leafStartColor;
		glm::vec3 leafEndColor;

		// At which depth should we start to add leafs (must be < than maxDepth)
		unsigned int depthStartingLeaf;
	} TreeGenerationData;

	// Base class for all procedural vegation generators
	// This way we can plug as much generators as we want without having to
	// touch the engine
	class ProceduralVegetation
	{
	protected:
		// Structure containing the generation info
		TreeGenerationData treeData;
	public:
		ProceduralVegetation(const TreeGenerationData & data);

		// Will return the tree mesh after computing it
		virtual Mesh * generate() = 0;
	};
}