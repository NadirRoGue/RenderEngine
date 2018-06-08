#pragma once

#include "Mesh.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Engine
{
	typedef struct TreeGenerationData
	{
		std::string treeName;
		unsigned long seed;

		unsigned int maxDepth;
		unsigned int startBranchingDepth;
		unsigned int maxBranchesSplit;

		bool rotateMainTrunk;
		glm::vec3 minBranchRotation;
		glm::vec3 maxBranchRotation;

		glm::vec3 scalingFactor;
		
		glm::vec3 startTrunkColor;
		glm::vec3 endTrunkColor;

		bool emissiveLeaf;
		glm::vec3 leafStartColor;
		glm::vec3 leafEndColor;
		unsigned int depthStartingLeaf;
	} TreeGenerationData;

	class ProceduralVegetation
	{
	protected:
		TreeGenerationData treeData;
	public:
		ProceduralVegetation(const TreeGenerationData & data);

		virtual Mesh * generate() = 0;
	};
}