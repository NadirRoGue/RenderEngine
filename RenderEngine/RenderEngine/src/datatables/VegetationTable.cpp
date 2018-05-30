#include "datatables/VegetationTable.h"
#include "datatables/MeshTable.h"

#include "vegetation/FractalTree.h"

Engine::VegetationTable * Engine::VegetationTable::INSTANCE = new Engine::VegetationTable();

Engine::VegetationTable & Engine::VegetationTable::getInstance()
{
	return *Engine::VegetationTable::INSTANCE;
}

Engine::VegetationTable::VegetationTable()
{

}

Engine::Mesh * Engine::VegetationTable::generateFractalTree(const Engine::TreeGenerationData & data, bool addToMeshTable)
{
	Engine::FractalTree ft(data);
	Engine::Mesh * tree = ft.generate();

	if (addToMeshTable)
	{
		Engine::MeshTable::getInstance().addMeshToCache(data.treeName, *tree);
	}

	return tree;
}

