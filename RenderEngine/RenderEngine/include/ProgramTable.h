/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include <map>
#include <string>

#include "Program.h"

namespace Engine
{
	class ProgramTable
	{
	private:
		static ProgramTable * INSTANCE;

		std::map<std::string, Program *> table;

	private:
		ProgramTable();

	public:
		static ProgramTable & getInstance();

		~ProgramTable();
		
		bool createProgram(Program * program, const char *vertexShader, const char *fragmentShader);
		Program * getProgramByName(std::string name);
		int getProgramIdByName(std::string & name);

		void destroyAllPrograms();
	};
}