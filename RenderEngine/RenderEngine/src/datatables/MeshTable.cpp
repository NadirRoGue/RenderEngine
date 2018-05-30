/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "datatables/MeshTable.h"

#include <assimp\cimport.h>
#include <assimp\postprocess.h>
#include <iostream>

Engine::MeshTable * Engine::MeshTable::INSTANCE = new Engine::MeshTable();

Engine::MeshTable & Engine::MeshTable::getInstance()
{
	return *INSTANCE;
}

Engine::MeshTable::MeshTable()
{
}

Engine::MeshTable::~MeshTable()
{
}

Engine::Mesh * Engine::MeshTable::getMesh(std::string filename)
{
	std::map<std::string, Engine::Mesh *>::iterator it = meshCache.find(filename);
	if (it != meshCache.end())
	{
		return meshCache[filename];
	}
	else
	{
		unsigned int flags = aiPostProcessSteps::aiProcess_GenUVCoords | aiPostProcessSteps::aiProcess_JoinIdenticalVertices;
		const aiScene * scene = aiImportFile(filename.c_str(), flags);

		if (!scene)
		{
			return NULL;
		}

		if (scene->HasMeshes())
		{
			aiMesh * rawMesh = scene->mMeshes[0];
			Mesh * m = new Mesh(rawMesh);

			meshCache[filename] = m;
			aiReleaseImport(scene);
			return meshCache[filename];
		}

		aiReleaseImport(scene);
	}

	return NULL;
}

void Engine::MeshTable::addMeshToCache(std::string name, Engine::Mesh & mesh)
{
	std::map<std::string, Engine::Mesh* >::iterator it = meshCache.find(name);
	if (it == meshCache.end())
	{
		meshCache[name] = new Engine::Mesh(mesh);
	}
}

void Engine::MeshTable::clean()
{
	std::map<std::string, Engine::Mesh* >::iterator it = meshCache.begin();
	while (it != meshCache.end())
	{
		it->second->releaseGPU();
		delete it->second;
		it++;
	}
}