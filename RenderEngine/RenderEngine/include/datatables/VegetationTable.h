/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Mesh.h"
#include "ProceduralVegetation.h"

namespace Engine
{
	/*
	 * Class in charge to give access and manage the different procedural vegetation
	 * engines available in the table
	 */
	class VegetationTable
	{
	private:
		static VegetationTable * INSTANCE;
	public:
		static VegetationTable & getInstance();
	private:
		VegetationTable();

	public:
		// Generates a tree mesh using a fractal algorithm
		Mesh * generateFractalTree(const TreeGenerationData & data, bool addToMeshTable =  true);
	};
}