/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "MeshLoader.h"

#include <assimp\cimport.h>
#include <assimp\postprocess.h>

Engine::MeshLoader * Engine::MeshLoader::INSTANCE = new Engine::MeshLoader();

Engine::MeshLoader & Engine::MeshLoader::getInstance()
{
	return *INSTANCE;
}

Engine::MeshLoader::MeshLoader()
{
}

Engine::MeshLoader::~MeshLoader()
{
	for (std::map<std::string, Engine::Mesh*>::iterator it = meshCache.begin(); it != meshCache.end(); it++)
	{
		it->second->~Mesh();
	}

	meshCache.clear();
}

Engine::Mesh * Engine::MeshLoader::getMesh(std::string filename)
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
			return meshCache[filename];
		}

		aiReleaseImport(scene);
	}

	return NULL;
}

void Engine::MeshLoader::addMeshToCache(std::string name, Engine::Mesh & mesh)
{
	std::map<std::string, Engine::Mesh* >::iterator it = meshCache.find(name);
	if (it == meshCache.end())
	{
		meshCache[name] = new Engine::Mesh(mesh);
	}
}

void Engine::MeshLoader::destroy()
{
	std::map<std::string, Engine::Mesh* >::iterator it = meshCache.begin();
	while (it != meshCache.end())
	{
		delete it->second;
		it++;
	}
}