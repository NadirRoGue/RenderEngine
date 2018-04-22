#pragma once

#include <glm\glm.hpp>

#include "Defines.h"
#include "Program.h"
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
		TerrainTile(MeshInstance * instance) : Object(instance)
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

	class Terrain
	{
	private:
		float tileWidth;
		unsigned int renderRadius;
		
		ProceduralTerrainProgram * shadingShader;
		ProceduralTerrainProgram * wireShader;

		Object * tileObject;
	public:
		Terrain();
		Terrain(float tileWidth, unsigned int renderRadius);
		Terrain(const Terrain & other);

		void render(Camera * camera);

	private:
		void initialize();
		void createTileMesh();
	};
}
