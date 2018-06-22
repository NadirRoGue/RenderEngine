#include "terraincomponents/TreeComponent.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"
#include "datatables/VegetationTable.h"

#include "CascadeShadowMaps.h"
#include "ProceduralVegetation.h"

#include <algorithm>
#include <random>

#include <iostream>

Engine::TreeComponent::TreeComponent()
	:Engine::TerrainComponent()
{

}

unsigned int Engine::TreeComponent::getRenderRadius()
{
	return 6;
}

void Engine::TreeComponent::initialize()
{
	// SHADERS
	fillShader = Engine::ProgramTable::getInstance().getProgram<Engine::TreeProgram>();

	wireShader = Engine::ProgramTable::getInstance().getProgram<Engine::TreeProgram>(Engine::TreeProgram::WIRE_MODE);

	shadowShader = Engine::ProgramTable::getInstance().getProgram<Engine::TreeProgram>(Engine::TreeProgram::SHADOW_MAP);

	activeShader = fillShader;

	// JITTERED TREE POSITIONS
	treesToSpawn = 12;
	size_t jitterSize = treesToSpawn % 2 != 0 ? treesToSpawn + 1 : treesToSpawn;
	int rows = (int)glm::ceil(sqrtf((float)jitterSize));
	int columns = rows;
	jitterPattern = new glm::vec2[rows * columns];

	std::uniform_real_distribution<float> d(0.0f, 1.0f);
	std::default_random_engine e(0);

	float jitterCellX = 1.0f / columns;
	float jitterCellY = 1.0f / rows;

	std::vector<glm::vec2> rawJitter;
	rawJitter.reserve(columns * rows);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			float x = d(e) * jitterCellX + jitterCellX * float(j);
			float y = d(e) * jitterCellY + jitterCellY * float(i);
			rawJitter.push_back(glm::vec2(x, y));
		}
	}

	std::shuffle(rawJitter.begin(), rawJitter.end(), std::default_random_engine(0));
	std::shuffle(rawJitter.begin(), rawJitter.end(), std::default_random_engine(5000));
	memcpy(jitterPattern, &rawJitter[0], sizeof(glm::vec2) * columns * rows);

	// TREE MESHES
	initTrees();
}

void Engine::TreeComponent::initTrees()
{
	// Procedural tree generation
	std::uniform_int_distribution<unsigned int> d(0, 50000);
	std::default_random_engine e(0);

	std::uniform_real_distribution<float> leafColor(0.0f, 1.0f);
	std::default_random_engine eLeaf(d(e) * d(e));

	std::uniform_real_distribution<float> trunkColor(0.0f, 1.0f);
	std::default_random_engine eTrunk(d(e) * d(e));

	for (int i = 0; i < 8; i++)
	{
		Engine::TreeGenerationData treeData;
		treeData.treeName = std::string("Tree_") + std::to_string(i);
		treeData.emissiveLeaf = leafColor(eLeaf) > 0.8f;
		treeData.startTrunkColor = trunkColor(eTrunk) >= 0.5f ? glm::vec3(0.2f, 0.2f, 0.0f) : glm::vec3(0.65f, 0.65f, 0.65f);
		treeData.endTrunkColor = treeData.startTrunkColor;
		treeData.leafStartColor = glm::vec3(1.0 - leafColor(eLeaf), 1.0 - leafColor(eLeaf), 1.0 - leafColor(eLeaf)) * 0.5f;
		treeData.leafEndColor = treeData.leafStartColor;
		treeData.maxBranchesSplit = 4;
		float rotFactor = leafColor(eLeaf) * 0.7f + 0.3f;
		treeData.maxBranchRotation = glm::vec3(45.0f, 10.0f, 10.0f) * rotFactor;
		treeData.minBranchRotation = glm::vec3(-45.0f, -10.0f, -10.0f) * rotFactor;
		treeData.maxDepth = 7;
		treeData.depthStartingLeaf = 6;
		treeData.rotateMainTrunk = false;
		treeData.scalingFactor = (glm::vec3(0.75, 1.0, 0.75) + glm::vec3(0.0f, (1.0f - rotFactor) * 0.25f, 0.0f));
		treeData.seed = d(e);
		treeData.startBranchingDepth = 2;

		Engine::Mesh * m = Engine::VegetationTable::getInstance().generateFractalTree(treeData, true);

		fillShader->configureMeshBuffers(m);
		wireShader->configureMeshBuffers(m);
		shadowShader->configureMeshBuffers(m);

		Engine::Object * tree = new Engine::Object(m);

		treeTypes.push_back(tree);
	}

	// Amount of each tree type to evenly spawn trees up to treesToSpawn
	size_t numTypeOfTrees = treeTypes.size();
	equalAmountOfTrees = treesToSpawn / numTypeOfTrees;
	equalAmountOfTrees = equalAmountOfTrees < 1 ? 1 : equalAmountOfTrees;
}

void Engine::TreeComponent::renderComponent(int i, int j, Engine::Camera * cam)
{
	float posX = i * scale;
	float posZ = j * scale;

	size_t numTypeOfTrees = treeTypes.size();
	Engine::CascadeShadowMaps & csm = Engine::CascadeShadowMaps::getInstance();

	size_t treeToSpawn = 0;
	unsigned int z = 0;
	while (z < treesToSpawn)
	{
		Engine::Object * randomTree = treeTypes[treeToSpawn % numTypeOfTrees];
		treeToSpawn++;
		randomTree->getMesh()->use();
		unsigned int k = 0;
		while (k < equalAmountOfTrees)
		{
			k++;
			z++;

			glm::vec2 & jitter = jitterPattern[z];
			float & uOffset = jitter.x;
			float & vOffset = jitter.y;

			float treePosX = posX + uOffset * scale;
			float treePosZ = posZ + vOffset * scale;

			randomTree->setTranslation(glm::vec3(treePosX, 0.0f, treePosZ));

			float u = abs(i + uOffset);
			float v = abs(j + vOffset);

			activeShader->setUniformTileUV(u, v);
			activeShader->setUniformLightDepthMat(csm.getDepthMatrix0() * randomTree->getModelMatrix());
			activeShader->setUniformLightDepthMat1(csm.getDepthMatrix1() * randomTree->getModelMatrix());
			activeShader->onRenderObject(randomTree, cam);

			glDrawElements(GL_TRIANGLES, randomTree->getMesh()->getNumFaces() * 3, GL_UNSIGNED_INT, (void*)0);
		}
	}
}

void Engine::TreeComponent::renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam)
{
	float posX = i * scale;
	float posZ = j * scale;

	size_t numTypeOfTrees = treeTypes.size();
	
	size_t treeToSpawn = 0;
	unsigned int z = 0;
	while (z < treesToSpawn)
	{
		Engine::Object * randomTree = treeTypes[treeToSpawn % numTypeOfTrees];
		treeToSpawn++;
		randomTree->getMesh()->use();
		unsigned int k = 0;
		while (k < equalAmountOfTrees)
		{
			k++;
			z++;
			glm::vec2 & jitter = jitterPattern[z];
			float & uOffset = jitter.x;
			float & vOffset = jitter.y;

			float treePosX = posX + uOffset * scale;
			float treePosZ = posZ + vOffset * scale;

			randomTree->setTranslation(glm::vec3(treePosX, 0.0f, treePosZ));

			float u = abs(i + uOffset);
			float v = abs(j + vOffset);

			shadowShader->setUniformTileUV(u, v);
			shadowShader->setUniformLightDepthMat(projection *  randomTree->getModelMatrix());
			shadowShader->onRenderObject(randomTree, cam);

			glDrawElements(GL_TRIANGLES, randomTree->getMesh()->getNumFaces() * 3, GL_UNSIGNED_INT, (void*)0);
		}
	}
}

void Engine::TreeComponent::notifyRenderModeChange(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_SHADED:
		activeShader = fillShader;
		break;
	case Engine::RenderMode::RENDER_MODE_WIRE:
		activeShader = wireShader;
	}
}

Engine::Program * Engine::TreeComponent::getActiveShader()
{
	return activeShader;
}

Engine::Program * Engine::TreeComponent::getShadowMapShader()
{
	return shadowShader;
}