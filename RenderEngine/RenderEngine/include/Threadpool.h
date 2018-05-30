#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>
#include <queue>

namespace Engine
{
	namespace Concurrent
	{
		class Runnable
		{
		public:
			virtual void run() = 0;
		};

		class ThreadPool
		{
		private:
			static ThreadPool * INSTANCE;
		private:
			std::list<std::thread> pool;
			std::queue<std::unique_ptr<Runnable>> tasks;

			std::mutex globalLock;
			std::condition_variable monitor;

			bool active;
			unsigned int poolSize;
		public:
			static ThreadPool & getInstance();
		private:
			ThreadPool();
		public:
			~ThreadPool();

			unsigned int getPoolSize() { return poolSize; }
			bool isActive() { return active; }
			void init();
			void addTask(std::unique_ptr<Runnable> task);
			void shutDown();
			void pollTask();
		};
	}
}