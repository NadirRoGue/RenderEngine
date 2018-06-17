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
	/**
	 * Class in charge of managing GPU programs (creation, access, clean up, etc.)
	 */
	class ProgramTable : public StorageTable
	{
	private:
		static ProgramTable * INSTANCE;

		// List of program factories
		std::map<std::string, ProgramFactory *> table;
	private:
		ProgramTable();

	public:
		static ProgramTable & getInstance();

		~ProgramTable();
		// Register a new program factory to allow the creation of the given shader type)
		void registerProgramFactory(std::string name, ProgramFactory * pf);

		// Attempts to return a program from the cache. If the combination name-parameters does not exist,
		// will create a new one
		Program * getProgramByName(std::string name, unsigned long long parameters = 0);

		// Returns a program of the given type already as an instance of such class
		template<typename T>
		T * getProgram(std::string name, unsigned long long parameters = 0)
		{
			Program * p = getProgramByName(name, parameters);
			if (p == NULL) return NULL;

			return static_cast<T*>(p);
		}

		// Releases the programs
		void clean();
	};
}