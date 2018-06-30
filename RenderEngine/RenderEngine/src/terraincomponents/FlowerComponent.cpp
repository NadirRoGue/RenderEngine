#include "terraincomponents/FlowerComponent.h"

#include "datatables/ProgramTable.h"
#include "datatables/MeshTable.h"
#include "datatables/VegetationTable.h"

#include "CascadeShadowMaps.h"
#include "ProceduralVegetation.h"

#include <random>

Engine::FlowerComponent::FlowerComponent()
	:Engine::TerrainComponent()
{

}

unsigned int Engine::FlowerComponent::getRenderRadius()
{
	return 3;
}

void Engine::FlowerComponent::initialize()
{
	// SHADERS
	fillShader = Engine::ProgramTable::getInstance().getProgram<Engine::TreeProgram>();

	wireShader = Engine::ProgramTable::getInstance().getProgram<Engine::TreeProgram>(Engine::TreeProgram::WIRE_MODE);

	pointShader = Engine::ProgramTable::getInstance().getProgram<Engine::TreeProgram>(Engine::TreeProgram::POINT_MODE);

	activeShader = fillShader;

	flowersToSpawn = 35;

	std::uniform_int_distribution<unsigned int> d(0, 50000);
	std::default_random_engine e(0);

	Engine::TreeGenerationData treeData;
	treeData.treeName = std::string("Flower");
	treeData.emissiveLeaf = false;
	treeData.startTrunkColor = glm::vec3(0.2f, 0.4f, 0.0f);
	treeData.endTrunkColor = treeData.startTrunkColor;
	treeData.leafStartColor = glm::vec3(1.0f, 0.1f, 0.1f);
	treeData.leafEndColor = glm::vec3(1.0f, 0.1f, 0.1f);
	treeData.maxBranchesSplit = 1;
	treeData.maxBranchRotation = glm::vec3(45.0f, 10.0f, 10.0f);
	treeData.minBranchRotation = glm::vec3(-45.0f, -10.0f, -10.0f);
	treeData.maxDepth = 2;
	treeData.depthStartingLeaf = 2;
	treeData.rotateMainTrunk = false;
	treeData.scalingFactor = glm::vec3(0.09f, 0.39f, 0.09f);
	treeData.seed = d(e);
	treeData.startBranchingDepth = 2;

	Engine::Mesh * m = Engine::VegetationTable::getInstance().generateFractalTree(treeData, true);

	fillShader->configureMeshBuffers(m);
	wireShader->configureMeshBuffers(m);

	flower = new Engine::Object(m);
}

void Engine::FlowerComponent::preRenderComponent()
{
	glBindVertexArray(flower->getMesh()->vao);
}

void Engine::FlowerComponent::renderComponent(int i, int j, Engine::Camera * cam)
{
	// We have to travel way to much to make this seed system not to work...
	unsigned int seed = (j << 16) | i;
	float posX = i * scale;
	float posZ = j * scale;

	std::uniform_real_distribution<float> dTerrain(0.0f, 1.0f);
	std::default_random_engine eTerrain(seed);

	Engine::CascadeShadowMaps & csm = Engine::CascadeShadowMaps::getInstance();

	const unsigned int numElements = flower->getMesh()->getNumFaces() * 3;

	unsigned int z = 0;
	while (z < flowersToSpawn)
	{
		z++;
		float uOffset = dTerrain(eTerrain);
		float vOffset = dTerrain(eTerrain);

		float treePosX = posX + uOffset * scale;
		float treePosZ = posZ + vOffset * scale;

		flower->setTranslation(glm::vec3(treePosX, 0.0f, treePosZ));

		float u = abs(i + uOffset);
		float v = abs(j + vOffset);

		activeShader->setUniformTileUV(u, v);
		activeShader->setUniformLightDepthMat(csm.getDepthMatrix0() * flower->getModelMatrix());
		activeShader->setUniformLightDepthMat1(csm.getDepthMatrix1() * flower->getModelMatrix());
		activeShader->onRenderObject(flower, cam);

		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, (void*)0);
	}
}

void Engine::FlowerComponent::renderShadow(const glm::mat4 & projection, int i, int j, Engine::Camera * cam)
{
	
}

void Engine::FlowerComponent::notifyRenderModeChange(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_SHADED:
		activeShader = fillShader;
		break;
	case Engine::RenderMode::RENDER_MODE_WIRE:
		activeShader = wireShader;
		break;
	case Engine::RenderMode::RENDER_MODE_POINT:
		activeShader = pointShader;
		break;
	}
}

Engine::Program * Engine::FlowerComponent::getActiveShader()
{
	return activeShader;
}

Engine::Program * Engine::FlowerComponent::getShadowMapShader()
{
	return NULL;
}