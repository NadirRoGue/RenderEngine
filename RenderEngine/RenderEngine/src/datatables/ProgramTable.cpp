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
	//delete INSTANCE;
}

bool Engine::ProgramTable::createProgram(Engine::Program * program, const char *vertexShader, const char *fragmentShader)
{
	// Duplicate program name
	if (table.find(program->getName()) != table.end())
	{
		std::cout << "ProgramTable: Duplicate program name " << program->getName() << std::endl;
		return false;
	}

	std::cout << "ProgramTable: Registered program " << program->getName() << std::endl;
	program->initialize(vertexShader, fragmentShader);
	table[program->getName()] = program;

	return true;
}

Engine::Program * Engine::ProgramTable::getProgramByName(std::string name)
{
	std::map<std::string, Engine::Program *>::iterator it = table.find(name);

	if (it == table.end())
	{
		std::cout << "ProgramTable: Tried to access unexistent program " << name << std::endl;
		return NULL;
	}

	return it->second;
}

int Engine::ProgramTable::getProgramIdByName(std::string & name)
{
	std::map<std::string, Program *>::iterator it = table.find(name);

	if (it == table.end())
	{
		return -1;
	}

	return it->second->getProgramId();
}

void Engine::ProgramTable::destroyAllPrograms()
{
	std::map<std::string, Engine::Program *>::iterator it = table.begin();
	while (it != table.end())
	{
		it->second->destroy();
		it++;
	}
}