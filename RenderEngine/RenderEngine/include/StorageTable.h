/*
* @author Nadir Román Guerrero
* @email nadir.ro.gue@gmail.com
*/
#pragma once

#include <list>

namespace Engine
{
	// All classes that loads/stores/manages objects must implements this class
	class StorageTable
	{
	public:
		virtual void clean() = 0;
	};

	// ===================================================================
	// Makes sure all Storage tables release their resources on exit
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