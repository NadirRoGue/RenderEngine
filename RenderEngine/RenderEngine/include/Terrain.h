#pragma once

#include <glm\glm.hpp>

#include "programs/ProceduralTerrainProgram.h"
#include "programs/ProceduralWaterProgram.h"
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
		
		ProceduralTerrainProgram * terrainShadingShader;
		ProceduralTerrainProgram * terrainWireShader;

		ProceduralTerrainProgram * terrainActiveShader;

		ProceduralWaterProgram * waterShadingShader;
		ProceduralWaterProgram * waterWireShader;

		ProceduralWaterProgram * waterActiveShader;

		Object * tileObject;
	public:
		Terrain();
		Terrain(float tileWidth, unsigned int renderRadius);
		Terrain(const Terrain & other);

		void render(Camera * camera);

		void notifyRenderModeUpdate(RenderMode mode);
	private:
		void initialize();
		void createTileMesh();
		void drawLayer(Engine::Camera * cam, Engine::ProceduralTerrainProgram * program, float yOffset = 0.0f);
	};
}
