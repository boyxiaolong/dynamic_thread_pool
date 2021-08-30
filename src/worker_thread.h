/*
 *
 * Copyright 2021 author zhaohailong.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

class task_callback;
class thread_pool;
class worker_thread
{
public:
	worker_thread(int thread_id, thread_pool* pthread_pool);
	virtual ~worker_thread();
	void run();
	void start();
	void stop();

	int get_thread_id();

private:
	std::atomic<bool> thread_runing_;
	std::thread* thd_ = NULL;
	int thread_id_;
	thread_pool* pthread_pool_;
};
typedef worker_thread* pworker_thread;
#endif

