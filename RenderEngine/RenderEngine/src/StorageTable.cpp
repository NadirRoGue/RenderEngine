#include "StorageTable.h"

Engine::TableManager * Engine::TableManager::INSTANCE = new Engine::TableManager();

Engine::TableManager & Engine::TableManager::getInstance()
{
	return *Engine::TableManager::INSTANCE;
}

Engine::TableManager::~TableManager()
{
	cleanUp();
}

void Engine::TableManager::registerTable(Engine::StorageTable * st)
{
	tables.push_back(st);
}

void Engine::TableManager::cleanUp()
{
	for (StorageTable * st : tables)
	{
		st->clean();
		//delete st;
	}

	tables.clear();
}