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
		MeshInstance(Mesh * mesh, std::string materialName);
		~MeshInstance();

		std::string & getMaterial();
		Mesh * getMesh();
	};
}