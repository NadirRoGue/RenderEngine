/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "MeshInstanceTable.h"

#include <vector>
#include <iostream>

#include "MeshLoader.h"
#include "ProgramTable.h"


using namespace Engine;

MeshInstanceTable * MeshInstanceTable::INSTANCE = new MeshInstanceTable();

MeshInstanceTable::MeshInstanceTable()
{

}

MeshInstanceTable::~MeshInstanceTable()
{

}

MeshInstanceTable & MeshInstanceTable::getInstance()
{
	return *INSTANCE;
}

bool MeshInstanceTable::instantiateMesh(std::string meshName, std::string programName)
{
	Mesh * mesh = MeshLoader::getInstance().getMesh(meshName);

	if (mesh == NULL)
	{
		std::cout << "MeshInstanceTable: Could not find mesh " << meshName << std::endl;
		return false;
	}

	Program * prog = ProgramTable::getInstance().getProgramByName(programName);
	if (prog == nullptr)
	{
		std::cout << "MeshInstanceTable: Could not create instance of mesh " << meshName << " -> Could not find program: " << programName << std::endl;
		return false;
	}

	MeshInstance * mi = new MeshInstance(mesh, programName);
	prog->configureMeshBuffers(mi);
	addMeshInstanceToCache(meshName, mi);

	return true;
}

void MeshInstanceTable::addMeshInstanceToCache(std::string meshName, MeshInstance * mi)
{
	std::map<std::string, std::map<std::string, MeshInstance *>>::iterator it = instanceTable.find(meshName);
	if (it != instanceTable.end())
	{
		std::map<std::string, MeshInstance *>::iterator it2 = it->second.find(mi->getMaterial());
		if (it2 != it->second.end())
		{
			//instanceTable[meshName][mi->getMaterial()] = mi;
			std::cout << "MeshInstanceTable: Duplicate mesh instance: Mesh name: " << meshName << " for material: " << mi->getMaterial() << std::endl;
		}
		else
		{
			it->second[mi->getMaterial()] = mi;
		}
	}
	else
	{
		instanceTable[meshName][mi->getMaterial()] = mi;
	}
}

MeshInstance * MeshInstanceTable::getMeshInstance(std::string meshName, std::string program)
{
	std::map<std::string, std::map<std::string, MeshInstance *>>::iterator it = instanceTable.find(meshName);
	if (it == instanceTable.end())
	{
		return NULL;
	}
	else
	{
		std::map<std::string, MeshInstance *> & meshCache = it->second;
		std::map<std::string, MeshInstance *>::iterator it2 = meshCache.find(program);

		if (it2 == meshCache.end())
		{
			return NULL;
		}
		else
		{
			return it2->second;
		}
	}
}

void MeshInstanceTable::destroy()
{
	std::map<std::string, std::map<std::string, MeshInstance *>>::iterator it;

	// Iterate over all the different meshes
	for (it = instanceTable.begin(); it != instanceTable.end(); it++)
	{
		std::map<std::string, MeshInstance *> meshInstances = it->second;
		std::map<std::string, MeshInstance *>::iterator instancesIt;

		// Iterate over all the used shaders for this mesh
		for (instancesIt = meshInstances.begin(); instancesIt != meshInstances.end(); instancesIt++)
		{
			Program * prog = ProgramTable::getInstance().getProgramByName(instancesIt->first);

			if (prog != nullptr)
			{
				prog->releaseProgramBuffers(instancesIt->second);
			}
		}
	}
}