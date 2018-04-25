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

		std::map<std::string, Program *> table;

	private:
		ProgramTable();

	public:
		static ProgramTable & getInstance();

		~ProgramTable();

		bool createProgram(Program * program);


		Program * getProgramByName(std::string name);
		int getProgramIdByName(std::string & name);

		void clean();
	};
}