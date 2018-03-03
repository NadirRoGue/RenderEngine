/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "MeshLoader.h"

#include <assimp\cimport.h>
#include <assimp\postprocess.h>

using namespace Engine;

MeshLoader * MeshLoader::INSTANCE = new MeshLoader();

MeshLoader & MeshLoader::getInstance()
{
	return *INSTANCE;
}

MeshLoader::MeshLoader() 
{
}

MeshLoader::~MeshLoader()
{
	for (std::map<std::string, Mesh*>::iterator it = meshCache.begin(); it != meshCache.end(); it++)
	{
		it->second->~Mesh();
	}

	meshCache.clear();

	//delete INSTANCE;
}

Mesh * MeshLoader::getMesh(std::string filename)
{
	std::map<std::string, Mesh *>::iterator it = meshCache.find(filename);
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

void MeshLoader::addMeshToCache(std::string name, Mesh & mesh)
{
	std::map<std::string, Mesh* >::iterator it = meshCache.find(name);
	if (it == meshCache.end())
	{
		meshCache[name] = new Mesh(mesh);
	}
}

void MeshLoader::destroy()
{
	std::map<std::string, Mesh* >::iterator it = meshCache.begin();
	while (it != meshCache.end())
	{
		delete it->second;
		it++;
	}
}