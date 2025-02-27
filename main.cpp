﻿#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <atomic>
#include <csignal>
#include <atomic>
#include <vector>
#include <list>
#include <string>
#include "stdio.h"
#include <memory>

#include "include/task_callback.h"
#include "include/worker_thread.h"
#include "include/thread_pool.h"

volatile std::sig_atomic_t gSignalStatus;
std::atomic_bool is_running(true);
void sig_handler(int sig) {
	is_running = false;
}

int main() {
	std::signal(SIGINT, sig_handler);
	
	int thread_num = 5;

	thread_pool tp(thread_num);

	tp.start();
	{
		auto fun = [](void* arg) {
			if (NULL == arg) {
				return;
			}

			int& str = *(static_cast<int*> (arg));
			printf("callback %d\n", str);
		};

		for (int i = 1; i < 9; ++i) {
			std::shared_ptr<task_callback> pdata(task_callback::create(fun, new int(i)));
			tp.push(pdata);
		}
	}

	while (is_running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	printf("main exit\n");
	return 0;
}