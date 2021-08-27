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

volatile std::sig_atomic_t gSignalStatus;
std::atomic_bool is_running(true);
void sig_handler(int sig)
{
	is_running = false;
}
int main()
{
	std::signal(SIGINT, sig_handler);
	int min_thread_num = 5;
	int max_thread_num = 10;
	int max_queue_size = 10;
	dynamic_thread_pool tp(min_thread_num, max_thread_num, max_queue_size);
	for (int i = 0; i < 999; ++i)
	{
		std::shared_ptr<task_callback> ptask_callback(new task_callback([](void* arg) {
			if (NULL == arg)
			{
				return;
			}

			std::string& str = *(static_cast<std::string*> (arg));
			printf("callback %s\n", str.c_str());
			}, new std::string("test" + std::to_string(i))));
		tp.push(ptask_callback);
	}

	while (is_running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
#ifdef _WIN32
	system("pause");
#endif
}