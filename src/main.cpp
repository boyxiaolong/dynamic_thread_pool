#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <atomic>
#include <csignal>
#include <atomic>
#include <vector>
#include <list>
#include <string>

#include "task_callback.h"
#include "worker_thread.h"
#include "dynamic_thread_pool.h"
#include "stdio.h"
#include <memory>
#include "thread_pool_2.h"

volatile std::sig_atomic_t gSignalStatus;
std::atomic_bool is_running(true);
void sig_handler(int sig)
{
	is_running = false;
}
int main()
{
	std::signal(SIGINT, sig_handler);
	int thread_num = 5;
	thread_pool_2 tp(thread_num);
	tp.start();
	{
		auto fun = [](void* arg) {
			if (NULL == arg)
			{
				return;
			}

			int& str = *(static_cast<int*> (arg));
			printf("callback %d\n", str);
		};
		for (int i = 1; i < 999; ++i)
		{
			ptask_callback pdata( task_callback::create(fun, new int(i)));
			bool res = tp.push(pdata);
			if (!res)
			{
				delete pdata;
			}
		}
	}

	while (is_running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	printf("main exit\n");
	return 0;
}