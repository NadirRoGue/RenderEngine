#pragma once

#include "Mesh.h"
#include "StorageTable.h"
#include "ProceduralVegetation.h"

namespace Engine
{
	class VegetationTable
	{
	private:
		static VegetationTable * INSTANCE;
	public:
		static VegetationTable & getInstance();
	private:
		VegetationTable();

	public:
		Mesh * generateFractalTree(const TreeGenerationData & data, bool addToMeshTable =  true);
	};
}