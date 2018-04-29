/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "datatables/ProgramTable.h"

#include <iostream>

Engine::ProgramTable * Engine::ProgramTable::INSTANCE = new Engine::ProgramTable();

Engine::ProgramTable & Engine::ProgramTable::getInstance()
{
	return *INSTANCE;
}

Engine::ProgramTable::ProgramTable()
{
}

Engine::ProgramTable::~ProgramTable()
{
	clean();
}

void Engine::ProgramTable::registerProgramFactory(std::string name, Engine::ProgramFactory * pf)
{
	if (pf != NULL)
	{
		table[name] = pf;
	}
}

Engine::Program * Engine::ProgramTable::getProgramByName(std::string name, unsigned long long parameters)
{
	std::map<std::string, Engine::ProgramFactory *>::iterator it = table.find(name);

	if (it == table.end())
	{
		std::cout << "ProgramTable: Tried to access unexistent program " << name << ", " << parameters << std::endl;
		return NULL;
	}
	else
	{
		Engine::ProgramFactory * factory = it->second;
		return factory->instantiateProgram(parameters);
	}
}

void Engine::ProgramTable::clean()
{
	std::map<std::string, Engine::ProgramFactory *>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->second != NULL)
		{
			it->second->clean();
			delete it->second;
		}
		it++;
	}
	table.clear();
	std::cout << "Program table cleared" << std::endl;
}