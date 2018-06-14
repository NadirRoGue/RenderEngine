#include "Terrain.h"

#include "Mesh.h"

#include "datatables/MeshTable.h"
#include "datatables/ProgramTable.h"
#include "datatables/VegetationTable.h"

#include "Scene.h"
#include "TimeAccesor.h"
#include "WorldConfig.h"

#include <random>

#include <iostream>

#include "CascadeShadowMaps.h"

Engine::Terrain::Terrain()
{
	tileWidth = 1.0f;
	renderRadius = 7;
	initialize();
}

Engine::Terrain::Terrain(float tileWidth, unsigned int renderRadius)
{
	this->tileWidth = tileWidth;
	this->renderRadius = renderRadius;
	initialize();
}

Engine::Terrain::~Terrain()
{
	delete tileObject;
}

// ====================================================================================================================

void Engine::Terrain::render(Engine::Camera * camera)
{
	unsigned int previousRadius = renderRadius;

	// Render cascade shadow maps
	for (unsigned int i = 0; i < Engine::CascadeShadowMaps::getInstance().getCascadeLevels(); i++)
	{
		Engine::CascadeShadowMaps::getInstance().saveCurrentFBO();
		Engine::CascadeShadowMaps::getInstance().beginShadowRender(i);
		
		// Re-bind tile, as we are rendering the trees in last place, thus changing the active vao
		tileObject->getMesh()->use();
		tiledRendering(camera, terrainShadowMapShader, &Terrain::terrainShadowMapRender);
		//tiledRendering(camera, waterShadowMapShader, &Terrain::waterShadowMapRender); // we dont really need to cast shadows from water...
		renderRadius = 4;
		tiledRendering(camera, treeShadowMapShader, &Terrain::treesShadowMapRender);
		renderRadius = previousRadius;
		
		Engine::CascadeShadowMaps::getInstance().endShadowRender();
	}

	// Bind tile to render terrain and water
	tileObject->getMesh()->use();

	// RENDER TERRAIN
	tiledRendering(camera, terrainActiveShader, &Terrain::terrainRender);

	// RENDER WATER
	// enable blending to produce water transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	tiledRendering(camera, waterActiveShader, &Terrain::waterRender);
	glDisable(GL_BLEND);

	// RENDER TREES
	renderRadius = renderRadius < 6? renderRadius : 6;
	tiledRendering(camera, treeActiveShader, &Terrain::treesRender);

	renderRadius = 3;
	flower->getMesh()->use();
	tiledRendering(camera, treeActiveShader, &Terrain::flowerRender);

	renderRadius = previousRadius;
}

// ====================================================================================================================

void Engine::Terrain::tiledRendering(Engine::Camera * camera, Program * prog, void (Terrain::*func)(Engine::Camera * cam, int i, int j))
{
	glm::vec3 cameraPosition = camera->getPosition();

	int x = -int((floor(cameraPosition.x)) / tileWidth);
	int y = -int((floor(cameraPosition.z)) / tileWidth);

	int xStart = x - renderRadius;
	int xEnd = x + renderRadius;
	int yStart = y - renderRadius;
	int yEnd = y + renderRadius;

	prog->use();
	prog->applyGlobalUniforms();

	// Culling parameters
	glm::vec3 fwd = camera->getForwardVector();
	fwd.y = 0;
	fwd = -glm::normalize(fwd);
	int px = -int(renderRadius), py = px;

	for (int i = xStart; i < xEnd; i++, px++)
	{
		for (int j = yStart; j < yEnd; j++, py++)
		{
			// Culling (skips almost half)
			glm::vec3 test(i - x, 0, j - y);
			if (abs(px) > 2 && abs(py) > 2 && glm::dot(glm::normalize(test), fwd) < 0.1f)
				continue;

			(this->*func)(camera, i, j);
		}
	}
}

// ====================================================================================================================

void Engine::Terrain::terrainShadowMapRender(Engine::Camera * camera, int i, int j)
{
	float poxX = i * tileWidth;
	float posZ = j * tileWidth;
	tileObject->setTranslation(glm::vec3(poxX, 0.0f, posZ));

	terrainShadowMapShader->setUniformGridPosition(i, j);
	terrainShadowMapShader->setUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance().getShadowProjectionMat() * tileObject->getModelMatrix());

	terrainShadowMapShader->onRenderObject(tileObject, camera);

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
}

/*
void Engine::Terrain::waterShadowMapRender(Engine::Camera * camera, int i, int j)
{
	float poxX = i * tileWidth;
	float posZ = j * tileWidth;
	tileObject->setTranslation(glm::vec3(poxX, 0.0f, posZ));

	waterShadowMapShader->setUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance().getShadowProjectionMat() * tileObject->getModelMatrix());

	waterShadowMapShader->onRenderObject(tileObject, camera);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
*/
void Engine::Terrain::terrainRender(Engine::Camera * camera, int i, int j)
{
	float poxX = i * tileWidth;
	float posZ = j * tileWidth;
	tileObject->setTranslation(glm::vec3(poxX, 0.0f, posZ));

	terrainActiveShader->setUniformGridPosition(i, j);
	terrainActiveShader->setUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance().getDepthMatrix0() * tileObject->getModelMatrix());
	terrainActiveShader->setUniformLightDepthMatrix1(Engine::CascadeShadowMaps::getInstance().getDepthMatrix1() * tileObject->getModelMatrix());

	terrainActiveShader->onRenderObject(tileObject, camera);

	glDrawElements(GL_PATCHES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::Terrain::waterRender(Engine::Camera * camera, int i, int j)
{
	float poxX = i * tileWidth;
	float posZ = j * tileWidth;
	tileObject->setTranslation(glm::vec3(poxX, Engine::Settings::waterHeight * tileWidth * 1.5f, posZ));

	waterActiveShader->setUniformGridPosition(i, j);
	waterActiveShader->setUniformLightDepthMatrix(Engine::CascadeShadowMaps::getInstance().getDepthMatrix0() * tileObject->getModelMatrix());
	waterActiveShader->setUniformLightDepthMatrix1(Engine::CascadeShadowMaps::getInstance().getDepthMatrix1() * tileObject->getModelMatrix());
	waterActiveShader->onRenderObject(tileObject, camera);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void Engine::Terrain::treesShadowMapRender(Engine::Camera * camera, int i, int j)
{
	float posX = i * tileWidth;
	float posZ = j * tileWidth;

	size_t numTypeOfTrees = treeTypes.size();
	Engine::CascadeShadowMaps & csm = Engine::CascadeShadowMaps::getInstance();

	size_t treeToSpawn = 0;
	unsigned int z = 0;
	while(z < treesToSpawn)
	{
		Engine::Object * randomTree = treeTypes[treeToSpawn % numTypeOfTrees];
		treeToSpawn++;
		randomTree->getMesh()->use();
		unsigned int k = 0;
		while(k < equalAmountOfTrees)
		{
			k++;
			z++;
			glm::vec2 & jitter = jitterPattern[z];
			float & uOffset = jitter.x;
			float & vOffset = jitter.y;

			float treePosX = posX + uOffset * tileWidth;
			float treePosZ = posZ + vOffset * tileWidth;

			randomTree->setTranslation(glm::vec3(treePosX, 0.0f, treePosZ));

			float u = abs(i + uOffset);
			float v = abs(j + vOffset);

			treeShadowMapShader->setUniformTileUV(u, v);
			treeShadowMapShader->setUniformLightDepthMat(csm.getShadowProjectionMat() *  randomTree->getModelMatrix());
			treeShadowMapShader->onRenderObject(randomTree, camera);

			glDrawElements(GL_TRIANGLES, randomTree->getMesh()->getNumFaces() * 3, GL_UNSIGNED_INT, (void*)0);
		}
	}
}

void Engine::Terrain::treesRender(Engine::Camera * camera, int i, int j)
{
	float posX = i * tileWidth;
	float posZ = j * tileWidth;

	size_t numTypeOfTrees = treeTypes.size();
	Engine::CascadeShadowMaps & csm = Engine::CascadeShadowMaps::getInstance();

	size_t treeToSpawn = 0;
	unsigned int z = 0;
	while(z < treesToSpawn)
	{
		Engine::Object * randomTree = treeTypes[treeToSpawn % numTypeOfTrees];
		treeToSpawn++;
		randomTree->getMesh()->use();
		unsigned int k = 0;
		while(k < equalAmountOfTrees)
		{
			k++;
			z++;

			glm::vec2 & jitter = jitterPattern[z];
			float & uOffset = jitter.x;
			float & vOffset = jitter.y;

			float treePosX = posX + uOffset * tileWidth;
			float treePosZ = posZ + vOffset * tileWidth;

			randomTree->setTranslation(glm::vec3(treePosX, 0.0f, treePosZ));

			float u = abs(i + uOffset);
			float v = abs(j + vOffset);

			treeActiveShader->setUniformTileUV(u, v);
			treeActiveShader->setUniformLightDepthMat(csm.getDepthMatrix0() * randomTree->getModelMatrix());
			treeActiveShader->setUniformLightDepthMat1(csm.getDepthMatrix1() * randomTree->getModelMatrix());
			treeActiveShader->onRenderObject(randomTree, camera);

			glDrawElements(GL_TRIANGLES, randomTree->getMesh()->getNumFaces() * 3, GL_UNSIGNED_INT, (void*)0);
		}
	}

}

void Engine::Terrain::flowerRender(Engine::Camera * cam, int i, int j)
{
	// We have to travel way to much to make this seed system not to work...
	unsigned int seed = (j << 16) | i;
	float posX = i * tileWidth;
	float posZ = j * tileWidth;

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

		float treePosX = posX + uOffset * tileWidth;
		float treePosZ = posZ + vOffset * tileWidth;

		flower->setTranslation(glm::vec3(treePosX, 0.0f, treePosZ));

		float u = abs(i + uOffset);
		float v = abs(j + vOffset);

		treeActiveShader->setUniformTileUV(u, v);
		treeActiveShader->setUniformLightDepthMat(csm.getDepthMatrix0() * flower->getModelMatrix());
		treeActiveShader->setUniformLightDepthMat1(csm.getDepthMatrix1() * flower->getModelMatrix());
		treeActiveShader->onRenderObject(flower, cam);

		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, (void*)0);
	}

}

// ====================================================================================================================

void Engine::Terrain::initialize()
{
	Engine::RenderableNotifier::getInstance().registerRenderable(this);

	treesToSpawn = 12;
	flowersToSpawn = 35;

	unsigned int jitterSize = treesToSpawn % 2 != 0 ? treesToSpawn + 1 : treesToSpawn;
	int rows = (int)glm::ceil(sqrtf((float)jitterSize));
	int columns = rows;
	jitterPattern = new glm::vec2[rows * columns];

	std::uniform_real_distribution<float> d(0.0f, 1.0f);
	std::default_random_engine e(0);

	float jitterCellX = 1.0f / columns;
	float jitterCellY = 1.0f / rows;

	int c = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			float x = d(e) * jitterCellX + jitterCellX * float(j);
			float y = d(e) * jitterCellY + jitterCellY * float(i);
			jitterPattern[c] = glm::vec2(x, y);
			c++;
		}
	}
	
	// TERRAIN SHADERS INSTANCING
	const std::string & terrainProgName = Engine::ProceduralTerrainProgram::PROGRAM_NAME;
	// shadow map
	terrainShadowMapShader = dynamic_cast<Engine::ProceduralTerrainProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(terrainProgName,
		Engine::ProceduralTerrainProgram::SHADOW_MAP)
	);
	// g-buffers
	terrainShadingShader = dynamic_cast<Engine::ProceduralTerrainProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(terrainProgName)
	);
	// wire mode
	terrainWireShader = dynamic_cast<Engine::ProceduralTerrainProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(terrainProgName,
		Engine::ProceduralTerrainProgram::WIRE_DRAW_MODE)
	);

	terrainActiveShader = terrainShadingShader;

	// WATER SHADERS INSTANCING
	const std::string & waterProgName = Engine::ProceduralWaterProgram::PROGRAM_NAME;
	// shadow map
	//waterShadowMapShader = dynamic_cast<Engine::ProceduralWaterProgram*>
	//(
	//	Engine::ProgramTable::getInstance().getProgramByName(waterProgName,
	//	Engine::ProceduralWaterProgram::SHADOW_MAP)
	//);
	// g-buffers
	waterShadingShader = dynamic_cast<Engine::ProceduralWaterProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(waterProgName)
	);
	// wire mode
	waterWireShader = dynamic_cast<Engine::ProceduralWaterProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(waterProgName,
		Engine::ProceduralWaterProgram::WIRE_DRAW_MODE)
	);

	waterActiveShader = waterShadingShader;

	// TREE SHADERS INSTANCING
	const std::string & treeProgName = Engine::TreeProgram::PROGRAM_NAME;
	// shading
	treeShader = dynamic_cast<Engine::TreeProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(treeProgName)
	);
	// wire mode
	treeWireShader = dynamic_cast<Engine::TreeProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(treeProgName, Engine::TreeProgram::WIRE_MODE)
	);
	// shadow map
	treeShadowMapShader = dynamic_cast<Engine::TreeProgram*>
	(
		Engine::ProgramTable::getInstance().getProgramByName(treeProgName, Engine::TreeProgram::SHADOW_MAP)
	);

	treeActiveShader = treeShader;

	// MESHES INSTANCING
	createTileMesh();

	// TREES INSTANCING
	addTrees();
}

void Engine::Terrain::createTileMesh()
{
	float vertices[12];
	vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
	vertices[3] = 1.0f; vertices[4] = 0.0f; vertices[5] = 0.0f;
	vertices[6] = 0.0f; vertices[7] = 0.0f; vertices[8] = 1.0f;
	vertices[9] = 1.0f; vertices[10] = 0.0f; vertices[11] = 1.0f;

	unsigned int faces[6];
	faces[0] = 0; faces[1] = 2; faces[2] = 1;
	faces[3] = 1; faces[4] = 2; faces[5] = 3;

	float normals[12];
	normals[0] = 0.0f; normals[1] = 1.0f; normals[2] = 0.0f;
	normals[3] = 0.0f; normals[4] = 1.0f; normals[5] = 0.0f;
	normals[6] = 0.0f; normals[7] = 1.0f; normals[8] = 0.0f;
	normals[9] = 0.0f; normals[10] = 1.0f; normals[11] = 0.0f;

	float uv[8];
	uv[0] = 0.0f; uv[1] = 0.0f;
	uv[2] = 1.0f; uv[3] = 0.0f;
	uv[4] = 0.0f; uv[5] = 1.0f;
	uv[6] = 1.0f; uv[7] = 1.0f;

	Engine::Mesh plane(2, 4, faces, vertices, 0, normals, uv, 0);
	Engine::MeshTable::getInstance().addMeshToCache("terrain_tile", plane);
	Engine::Mesh * planeMesh = Engine::MeshTable::getInstance().getMesh("terrain_tile");
	
	terrainShadingShader->configureMeshBuffers(planeMesh);
	waterShadingShader->configureMeshBuffers(planeMesh);

	tileObject = new Engine::Object(planeMesh);
	if(tileWidth != 1.0f)
		tileObject->setScale(glm::vec3(tileWidth, tileWidth, tileWidth));
}

void Engine::Terrain::addTrees()
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
		treeData.startTrunkColor = trunkColor(eTrunk) >= 0.5f? glm::vec3(0.2f, 0.2f, 0.0f) : glm::vec3(0.65f, 0.65f, 0.65f);
		treeData.endTrunkColor = treeData.startTrunkColor;
		treeData.leafStartColor = glm::vec3(1.0 - leafColor(eLeaf), 1.0 - leafColor(eLeaf), 1.0 - leafColor(eLeaf));
		treeData.leafEndColor = treeData.leafStartColor;
		treeData.maxBranchesSplit = 4;
		float rotFactor = leafColor(eLeaf) * 0.5f + 0.5f;
		treeData.maxBranchRotation = glm::vec3(45.0f, 10.0f, 10.0f) * rotFactor;
		treeData.minBranchRotation = glm::vec3(-45.0f, -10.0f, -10.0f) * rotFactor;
		treeData.maxDepth = 7;
		treeData.depthStartingLeaf = 6;
		treeData.rotateMainTrunk = false;
		treeData.scalingFactor = glm::vec3(0.75, 1.0, 0.75) + glm::vec3(0.0f, (1.0f - rotFactor) * 0.25f, 0.0f);
		treeData.seed = d(e);
		treeData.startBranchingDepth = 2;

		Engine::Mesh * m = Engine::VegetationTable::getInstance().generateFractalTree(treeData, true);

		std::cout << "Tree: " << m->getNumFaces() << " faces, " << m->getNumVertices() << " vertices" << std::endl;

		treeShader->configureMeshBuffers(m);
		treeWireShader->configureMeshBuffers(m);
		treeShadowMapShader->configureMeshBuffers(m);

		Engine::Object * tree = new Engine::Object(m);
		
		treeTypes.push_back(tree);
	}

	// Amount of each tree type to evenly spawn trees up to treesToSpawn
	size_t numTypeOfTrees = treeTypes.size();
	equalAmountOfTrees = treesToSpawn / numTypeOfTrees;
	equalAmountOfTrees = equalAmountOfTrees < 1 ? 1 : equalAmountOfTrees;
	
	// Procedural flower generation
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

	std::cout << "Flower: " << m->getNumFaces() << " faces, " << m->getNumVertices() << " vertices" << std::endl;

	treeShader->configureMeshBuffers(m);
	treeWireShader->configureMeshBuffers(m);
	treeShadowMapShader->configureMeshBuffers(m);

	flower = new Engine::Object(m);
}

// ====================================================================================================================

void Engine::Terrain::notifyRenderModeUpdate(Engine::RenderMode mode)
{
	switch (mode)
	{
	case Engine::RenderMode::RENDER_MODE_WIRE:
		terrainActiveShader = terrainWireShader;
		waterActiveShader = waterWireShader;
		treeActiveShader = treeWireShader;
		break;
	default:
		terrainActiveShader = terrainShadingShader;
		waterActiveShader = waterShadingShader;
		treeActiveShader = treeShader;
		break;
	}
}