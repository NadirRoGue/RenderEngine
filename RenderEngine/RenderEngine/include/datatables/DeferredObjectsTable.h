/**
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/

#pragma once

#include "StorageTable.h"
#include "DeferredRenderObject.h"

#include <list>

namespace Engine
{
	/*
	 * Class in charge to manage the needed FBO (resize, cleanup, etc)
	 */
	class DeferredObjectsTable : public StorageTable
	{
	private:
		static DeferredObjectsTable * INSTANCE;
	private:
		// List of created FBOs
		std::list<DeferredRenderObject *> deferredObjects;
	public:
		static DeferredObjectsTable & getInstance();
	private:
		DeferredObjectsTable();
	public:
		// Registers a new FBO. Automatically called on FBO creation
		void registerDeferredObject(DeferredRenderObject * ro);
		// Resizes all registered FBOs
		void onResize(int width, int height);
		// Releases the FBOs from the GPU and CPU
		void clean();
	};
}