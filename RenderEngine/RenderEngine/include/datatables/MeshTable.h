/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Mesh.h"
#include <vector>
#include <map>
#include <assimp\scene.h>

namespace Engine
{
	class MeshTable
	{
	private:
		static MeshTable * INSTANCE;

		std::map<std::string, Mesh *> meshCache;

	private:
		MeshTable();

	public:
		static MeshTable & getInstance();

		~MeshTable();

		Mesh * getMesh(std::string fileName);

		void addMeshToCache(std::string name, Mesh & mesh);

		void destroy();
	};
}