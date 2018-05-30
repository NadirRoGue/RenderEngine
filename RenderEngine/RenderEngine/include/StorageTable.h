#pragma once

#include <list>

namespace Engine
{
	class StorageTable
	{
	public:
		virtual void clean() = 0;
	};

	// ===================================================================

	class TableManager
	{
	private:
		static TableManager * INSTANCE;
	private:
		std::list<StorageTable *> tables;

	public:
		static TableManager & getInstance();

	private:
		TableManager(){ }
	public:
		~TableManager();

		void registerTable(StorageTable * st);
		void cleanUp();
	};
}