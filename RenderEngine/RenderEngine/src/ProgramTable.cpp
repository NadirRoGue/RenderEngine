/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#include "ProgramTable.h"

#include <iostream>

using namespace Engine;

ProgramTable * ProgramTable::INSTANCE = new ProgramTable();

ProgramTable & ProgramTable::getInstance()
{
	return *INSTANCE;
}

ProgramTable::ProgramTable()
{
}

ProgramTable::~ProgramTable()
{
	//delete INSTANCE;
}

bool ProgramTable::createProgram(Program * program, const char *vertexShader, const char *fragmentShader)
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

Program * ProgramTable::getProgramByName(std::string name)
{
	std::map<std::string, Program *>::iterator it = table.find(name);

	if (it == table.end())
	{
		std::cout << "ProgramTable: Tried to access unexistent program " << name << std::endl;
		return NULL;
	}

	return it->second;
}

int ProgramTable::getProgramIdByName(std::string & name)
{
	std::map<std::string, Program *>::iterator it = table.find(name);

	if (it == table.end())
	{
		return -1;
	}

	return it->second->getProgramId();
}

void ProgramTable::destroyAllPrograms()
{
	std::map<std::string, Program *>::iterator it = table.begin();
	while (it != table.end())
	{
		it->second->destroy();
		it++;
	}
}