/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include <string>

#include "Program.h"
#include "StorageTable.h"

namespace Engine
{
	class ProgramTable : public StorageTable
	{
	private:
		static ProgramTable * INSTANCE;

		//std::map<std::string, Program *> table;
		std::map<std::string, ProgramFactory *> table;
	private:
		ProgramTable();

	public:
		static ProgramTable & getInstance();

		~ProgramTable();
		void registerProgramFactory(std::string name, ProgramFactory * pf);

		Program * getProgramByName(std::string name, unsigned long long parameters = 0);

		void clean();
	};
}