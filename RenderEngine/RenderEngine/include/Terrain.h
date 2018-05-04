#pragma once

#include <glm\glm.hpp>

#include "programs/ProceduralTerrainProgram.h"
#include "programs/ProceduralWaterProgram.h"
#include "Object.h"
#include "Camera.h"
#include "DeferredRenderObject.h"

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

		ProceduralWaterProgram * waterShadowMapShader;
		ProceduralWaterProgram * waterShadingShader;
		ProceduralWaterProgram * waterWireShader;

		ProceduralWaterProgram * waterActiveShader;

		Object * tileObject;

		glm::mat4 lightProjMatrix;
		glm::mat4 lightDepthMat;
		glm::mat4 biasMat;

		DeferredRenderObject * shadowMap;
		TextureInstance * depthTexture;
	public:
		Terrain();
		Terrain(float tileWidth, unsigned int renderRadius);
		~Terrain();

		void render(Camera * camera);

		void notifyRenderModeUpdate(RenderMode mode);
	private:
		void initialize();
		void createTileMesh();

		void tiledRendering(Camera * cam, Program * prog, void (Terrain::*func)(Camera * cam, int i, int j));

		void terrainShadowMapRender(Camera * cam, int i, int j);
		void waterShadowMapRender(Camera * cam, int i, int j);
		void terrainRender(Camera * cam, int i, int j);
		void waterRender(Camera * cam, int i, int j);
	};
}
