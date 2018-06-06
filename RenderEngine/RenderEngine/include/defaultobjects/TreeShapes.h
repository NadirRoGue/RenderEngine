/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Mesh.h"

namespace Engine
{
	/**
	 * Holds the data needed to create a indexed, open at the bottom, cube
	 * used as base shape to build the procedural vegetation
	 */
	namespace TrunkData
	{
		const unsigned int triangleIndex[] = 
		{
			//Face z = -1
			0,4,1,			1,4,5,
			//Face z = 1
			3,7,2,			2,7,6,
			//Face x = -1
			2,6,0,			0,6,4,
			//Face x = -1
			1,5,3,		3,5,7,
			//Face y = 1
			4,6,5,		5,6,7,
		};

		const float vertexPos[] = 
		{
			//Face y = -1
			-1.0f,	0.0f,	 -1.0f, //0
			1.0f,	0.0f,	 -1.0f, //1
			-1.0f,	0.0f,	 1.0f, //2
			1.0f,	0.0f,	 1.0f, //3

			//Face z = -1		   
			-1.0f,	1.0f,	-1.0f, //4
			1.0f,	1.0f,	-1.0f, //5
			-1.0f,	1.0f,	1.0f, //6
			1.0f,	1.0f,	1.0f, //7
		};
	}

	// Creates a trunk piece mesh
	Mesh CreateTrunk()
	{
		return Mesh
		(
			10,
			8,
			TrunkData::triangleIndex,
			TrunkData::vertexPos,
			0,
			0,
			0,
			0,
			0
		);
	}

	/**
	 * UNUSED - Holds the data to create triangular leafs for
	 * the procedural vegetation
	 */
	namespace LeafData
	{
		const unsigned int triangleIndex[] =
		{
			0,1,2,
			1,0,2
		};

		const float vertexPos[] =
		{
			-0.5f,0.0f,0.0f,
			0.5f,0.0f,0.0f,
			0.0f,1.0f,0.0f
		};
	}

	Mesh createLeaf()
	{
		return Mesh
		(
			2,
			3,
			LeafData::triangleIndex,
			LeafData::vertexPos,
			0,
			0,
			0,
			0,
			0
		);
	}
}