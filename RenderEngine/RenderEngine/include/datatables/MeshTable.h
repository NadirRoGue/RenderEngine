/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "Mesh.h"
#include <vector>
#include <map>
#include <assimp\scene.h>

#include "StorageTable.h"

namespace Engine
{
	/*
	 * Class in charge of manage the instanced meshes (access, cleanup, etc.)
	 */
	class MeshTable : public StorageTable
	{
	private:
		static MeshTable * INSTANCE;

		// List of meshes
		std::map<std::string, Mesh *> meshCache;

	private:
		MeshTable();

	public:
		static MeshTable & getInstance();

		~MeshTable();

		// Returns the mesh described by filename. If it is not present, will attempt to load from disk
		Mesh * getMesh(std::string fileName);

		// Manually place a mesh into the cache
		void addMeshToCache(std::string name, const Mesh & mesh);

		// Clean all meshes (GPU & CPU)
		void clean();
	};
}