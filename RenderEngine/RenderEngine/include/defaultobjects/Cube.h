#pragma once

#include "Mesh.h"

namespace Engine
{
	namespace CubeData
	{
		const int cubeNVertex = 24; 
		const int cubeNTriangleIndex = 12; 

		const unsigned int cubeTriangleIndex[] = {
			//Face z = 1
			0,1,2,			1,3,2,
			//Face z = -1
			4,6,5,			5,6,7,
			//Face x = 1
			8,10,9,			9,10,11,
			//Face x = -1
			12,13,14,		13,15,14,
			//Face y = 1
			16,17,18,		17,19,18,
			//Face y = -1
			20,22,21,		21,22,23,
		};

		const float cubeVertexPos[] = {
			//Face z = 1
			-1.0f,	-1.0f,	 1.0f, //0
			 1.0f,	-1.0f,	 1.0f, //1
			-1.0f,	 1.0f,	 1.0f, //2
			 1.0f,	 1.0f,	 1.0f, //3

			//Face z = -1		   
			-1.0f,	-1.0f,	-1.0f, //4
			 1.0f,	-1.0f,	-1.0f, //5
			-1.0f,	 1.0f,	-1.0f, //6
			 1.0f,	 1.0f,	-1.0f, //7

			//Face x = 1		   
			1.0f,	-1.0f,	-1.0f, //8
			1.0f,	-1.0f,	 1.0f, //9
			1.0f,	 1.0f,	-1.0f, //10
			1.0f,	 1.0f,	 1.0f, //11

			//Face x = -1		   
			-1.0f,	-1.0f,	-1.0f, //12
			-1.0f,	-1.0f,	 1.0f, //13
			-1.0f,	 1.0f,	-1.0f, //14
			-1.0f,	 1.0f,	 1.0f, //15

			//Face y = 1		   
			-1.0f,	 1.0f,	-1.0f, //16
			-1.0f,	 1.0f,	 1.0f, //17
			 1.0f,	 1.0f,	-1.0f, //18
			 1.0f,	 1.0f,	 1.0f, //19

			//Face y = -1		   
			-1.0f,	-1.0f,	-1.0f, //20
			-1.0f,	-1.0f,	 1.0f, //21
			 1.0f,	-1.0f,	-1.0f, //22
			 1.0f,	-1.0f,	 1.0f  //23
		};

		const float cubeVertexNormal[] = {
			//Face z = 1
			0.0f,	0.0f,	 1.0f,
			0.0f,	0.0f,	 1.0f,
			0.0f,	0.0f,	 1.0f,
			0.0f,	0.0f,	 1.0f,

			//Face z = -1		   
			0.0f,	0.0f,	-1.0f,
			0.0f,	0.0f,	-1.0f,
			0.0f,	0.0f,	-1.0f,
			0.0f,	0.0f,	-1.0f,

			//Face x = 1		   
			1.0f,	0.0f,	 0.0f,
			1.0f,	0.0f,	 0.0f,
			1.0f,	0.0f,	 0.0f,
			1.0f,	0.0f,	 0.0f,

			//Face x = -1		   
			-1.0f,	0.0f,	 0.0f,
			-1.0f,	0.0f,	 0.0f,
			-1.0f,	0.0f,	 0.0f,
			-1.0f,	0.0f,	 0.0f,

			//Face y = 1		   
			0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,

			//Face y = -1		   
			0.0f,	-1.0f,	0.0f,
			0.0f,	-1.0f,	0.0f,
			0.0f,	-1.0f,	0.0f,
			0.0f,	-1.0f,	0.0f
		};

		const float cubeVertexColor[] = {
			//Face z = 1
			0.0f,	0.0f,	 1.0f,
			0.0f,	0.0f,	 1.0f,
			0.0f,	0.0f,	 1.0f,
			0.0f,	0.0f,	 1.0f,

			//Face z = -1		   
			0.6f,	0.6f,	 1.0f,
			0.6f,	0.6f,	 1.0f,
			0.6f,	0.6f,	 1.0f,
			0.6f,	0.6f,	 1.0f,

			//Face x = 1		   
			1.0f,	0.0f,	 0.0f,
			1.0f,	0.0f,	 0.0f,
			1.0f,	0.0f,	 0.0f,
			1.0f,	0.0f,	 0.0f,

			//Face x = -1		   
			1.0f,	0.6f,	 0.6f,
			1.0f,	0.6f,	 0.6f,
			1.0f,	0.6f,	 0.6f,
			1.0f,	0.6f,	 0.6f,

			//Face y = 1		   
			0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,

			//Face y = -1		   
			0.6f,	1.0f,	0.6f,
			0.6f,	1.0f,	0.6f,
			0.6f,	1.0f,	0.6f,
			0.6f,	1.0f,	0.6f
		};

		const float cubeVertexTexCoord[] = {
			//Face z = 1
			 0.0f, 0.0f,
			 1.0f, 0.0f,
			 0.0f, 1.0f,
			 1.0f, 1.0f,

			 //Face z = -1
			 0.0f, 1.0f,
			 1.0f, 1.0f,
			 0.0f, 0.0f,
			 1.0f, 0.0f,

			 //Face x = 1	
			 0.0f,	1.0f,
			 1.0f,	1.0f,
			 0.0f,	0.0f,
			 1.0f,	0.0f,

			 //Face x = -1
			 0.0f,	0.0f,
			 1.0f,	0.0f,
			 0.0f,	1.0f,
			 1.0f,	1.0f,

			 //Face y = 1	
			 0.0f, 1.0f,
			 0.0f, 0.0f,
			 1.0f, 1.0f,
			 1.0f, 0.0f,

			 //Face y = -1
			 0.0f, 0.0f,
			 0.0f, 1.0f,
			 1.0f, 0.0f,
			 1.0f, 1.0f,
		};

		const float cubeVertexTangent[] = {
			//Face z = 1
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,

			//Face z = -1				
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,

			//Face x = 1				
			0.0f,	0.0f,	-1.0f,
			0.0f,	0.0f,	-1.0f,
			0.0f,	0.0f,	-1.0f,
			0.0f,	0.0f,	-1.0f,

			//Face x = -1				
			0.0f,	0.0f,	1.0f,
			0.0f,	0.0f,	1.0f,
			0.0f,	0.0f,	1.0f,
			0.0f,	0.0f,	1.0f,

			//Face y = 1	   
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,

			//Face y = -1					   
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
		};
	}

	Mesh CreateCube()
	{
		return Mesh
		(
			CubeData::cubeNTriangleIndex,
			CubeData::cubeNVertex,
			CubeData::cubeTriangleIndex,
			CubeData::cubeVertexPos,
			CubeData::cubeVertexColor,
			CubeData::cubeVertexNormal,
			CubeData::cubeVertexTexCoord,
			CubeData::cubeVertexTangent
		);
	}
}