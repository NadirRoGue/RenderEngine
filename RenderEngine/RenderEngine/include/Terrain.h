#pragma once

#include <vector>

#include "programs/ProceduralTerrainProgram.h"
#include "programs/ProceduralWaterProgram.h"
#include "programs/TreeProgram.h"
#include "Object.h"
#include "Camera.h"

namespace Engine
{
	class TerrainTile : public Object
	{
	private:
		unsigned int gridPosI;
		unsigned int gridPosJ;
	public:
		TerrainTile(Mesh * instance) : Object(instance)
		{
		}

		void setGridPosI(unsigned int i)
		{
			gridPosI = i;
		}

		void setGridPosJ(unsigned int j)
		{
			gridPosJ = j;
		}

		unsigned int getGridPosI()
		{
			return gridPosI;
		}

		unsigned int getGridPosJ()
		{
			return gridPosJ;
		}
	};

	class Terrain : public IRenderable
	{
	private:
		float tileWidth;
		unsigned int renderRadius;
		
		ProceduralTerrainProgram * terrainShadowMapShader;
		ProceduralTerrainProgram * terrainShadingShader;
		ProceduralTerrainProgram * terrainWireShader;

		ProceduralTerrainProgram * terrainActiveShader;

		//ProceduralWaterProgram * waterShadowMapShader;
		ProceduralWaterProgram * waterShadingShader;
		ProceduralWaterProgram * waterWireShader;

		ProceduralWaterProgram * waterActiveShader;

		TreeProgram * treeShadowMapShader;
		TreeProgram * treeShader;
		TreeProgram * treeWireShader;

		TreeProgram * treeActiveShader;

		Object * tileObject;

		std::vector<Object *> treeTypes;
		Object * flower;
		unsigned int treesToSpawn;
		unsigned int equalAmountOfTrees;
		unsigned int flowersToSpawn;
		glm::vec2 * jitterPattern;
	public:
		Terrain();
		Terrain(float tileWidth, unsigned int renderRadius);
		~Terrain();

		void render(Camera * camera);

		void notifyRenderModeUpdate(RenderMode mode);
	private:
		void initialize();
		void createTileMesh();
		void addTrees();

		void tiledRendering(Camera * cam, Program * prog, void (Terrain::*func)(Camera * cam, int i, int j));

		void terrainShadowMapRender(Camera * cam, int i, int j);
		//void waterShadowMapRender(Camera * cam, int i, int j);
		void treesShadowMapRender(Camera * cam, int i, int j);
		void terrainRender(Camera * cam, int i, int j);
		void waterRender(Camera * cam, int i, int j);
		void treesRender(Camera * cam, int i, int j);
		void flowerRender(Camera * cam, int i, int j);
	};
}
