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
	class MeshLoader
	{
	private:
		static MeshLoader * INSTANCE;

		std::map<std::string, Mesh *> meshCache;

	private:
		MeshLoader();

	public:
		static MeshLoader & getInstance();

		~MeshLoader();

		Mesh * getMesh(std::string fileName);

		void addMeshToCache(std::string name, Mesh & mesh);

		void destroy();
	};
}