/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include <list>
#include <string>

#include "StorageTable.h"
#include "instances/MeshInstance.h"

namespace Engine
{
	class MeshInstanceTable : public StorageTable
	{
	private:
		static MeshInstanceTable * INSTANCE;

		std::map<std::string, std::map<std::string, MeshInstance *>> instanceTable;

	private:
		MeshInstanceTable();

		void addMeshInstanceToCache(std::string name, MeshInstance * mi);
	public:
		static MeshInstanceTable & getInstance();

		~MeshInstanceTable();

		bool instantiateMesh(std::string name, std::string programName);
		MeshInstance * getMeshInstance(std::string meshName, std::string program);

		void clean();
	};
}