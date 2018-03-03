/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <glm/glm.hpp>

#include "Mesh.h"

namespace Engine
{
	class MeshInstance
	{
	private:
		std::string material;
		Mesh * mesh;

	public:
		unsigned int vao;
		unsigned int vboFaces;
		unsigned int vboVertices;
		unsigned int vboNormals;
		unsigned int vboColors;
		unsigned int vboUVs;
		unsigned int vboTangents;

	public:
		MeshInstance(Mesh * mesh, std::string materialName);
		~MeshInstance();

		std::string & getMaterial();
		Mesh * getMesh();
	};
}