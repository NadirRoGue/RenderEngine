#pragma once

#include "StorageTable.h"
#include "DeferredRenderObject.h"

#include <list>

namespace Engine
{
	class DeferredObjectsTable : public StorageTable
	{
	private:
		static DeferredObjectsTable * INSTANCE;
	private:
		std::list<DeferredRenderObject *> deferredObjects;
	public:
		static DeferredObjectsTable & getInstance();
	private:
		DeferredObjectsTable();
	public:
		void registerDeferredObject(DeferredRenderObject * ro);
		void onResize(int width, int height);
		void clean();
	};
}