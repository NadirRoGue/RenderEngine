/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "datatables/MeshInstanceTable.h"

#include <vector>
#include <iostream>

#include "Program.h"
#include "datatables/MeshTable.h"
#include "datatables/ProgramTable.h"

Engine::MeshInstanceTable * Engine::MeshInstanceTable::INSTANCE = new Engine::MeshInstanceTable();

Engine::MeshInstanceTable::MeshInstanceTable()
{

}

Engine::MeshInstanceTable::~MeshInstanceTable()
{

}

Engine::MeshInstanceTable & Engine::MeshInstanceTable::getInstance()
{
	return *INSTANCE;
}

bool Engine::MeshInstanceTable::instantiateMesh(std::string meshName, std::string programName)
{
	Engine::Mesh * mesh = Engine::MeshTable::getInstance().getMesh(meshName);

	if (mesh == NULL)
	{
		std::cout << "MeshInstanceTable: Could not find mesh " << meshName << std::endl;
		return false;
	}

	Engine::Program * prog = Engine::ProgramTable::getInstance().getProgramByName(programName);
	if (prog == nullptr)
	{
		std::cout << "MeshInstanceTable: Could not create instance of mesh " << meshName << " -> Could not find program: " << programName << std::endl;
		return false;
	}

	Engine::MeshInstance * mi = new Engine::MeshInstance(mesh, programName);
	prog->configureMeshBuffers(mi);
	addMeshInstanceToCache(meshName, mi);

	return true;
}

void Engine::MeshInstanceTable::addMeshInstanceToCache(std::string meshName, Engine::MeshInstance * mi)
{
	std::map<std::string, std::map<std::string, Engine::MeshInstance *>>::iterator it = instanceTable.find(meshName);
	if (it != instanceTable.end())
	{
		std::map<std::string, Engine::MeshInstance *>::iterator it2 = it->second.find(mi->getMaterial());
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

Engine::MeshInstance * Engine::MeshInstanceTable::getMeshInstance(std::string meshName, std::string program)
{
	std::map<std::string, std::map<std::string, Engine::MeshInstance *>>::iterator it = instanceTable.find(meshName);
	if (it == instanceTable.end())
	{
		return NULL;
	}
	else
	{
		std::map<std::string, Engine::MeshInstance *> & meshCache = it->second;
		std::map<std::string, Engine::MeshInstance *>::iterator it2 = meshCache.find(program);

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

void Engine::MeshInstanceTable::destroy()
{
	instanceTable.clear();
}