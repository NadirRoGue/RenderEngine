#include "datatables/DeferredObjectsTable.h"

Engine::DeferredObjectsTable * Engine::DeferredObjectsTable::INSTANCE = new Engine::DeferredObjectsTable();

Engine::DeferredObjectsTable & Engine::DeferredObjectsTable::getInstance()
{
	return *INSTANCE;
}

Engine::DeferredObjectsTable::DeferredObjectsTable()
{
}

void Engine::DeferredObjectsTable::registerDeferredObject(Engine::DeferredRenderObject * ro)
{
	deferredObjects.push_back(ro);
}

void Engine::DeferredObjectsTable::onResize(int width, int height)
{
	for (auto ro : deferredObjects)
	{
		ro->resizeFBO(width, height);
	}
}

void Engine::DeferredObjectsTable::clean()
{
	for (auto ro : deferredObjects)
	{
		delete ro;
	}

	deferredObjects.clear();
}