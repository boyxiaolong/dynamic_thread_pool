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

#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <list>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include "utils.h"

class task_callback;
class worker_thread;

class thread_pool
{
public:
	
	thread_pool(int min_thread_num);

	~thread_pool();

	void start();

	void stop();

	bool push(task_callback_type t);

	task_callback_type get_task();

private:
	bool _gen_threads();

private:

	typedef std::shared_ptr<worker_thread> worker_thread_type;
	typedef std::list<worker_thread_type > thread_vec_type;

	int thread_num_;

	thread_vec_type thd_vec_;

	std::mutex thread_lock_;

	std::atomic<bool> is_runing_;

	std::mutex task_lock_;

	std::queue<task_callback_type> tasks_;

	std::condition_variable task_con_;

	std::atomic<int> total_task_size_;
};

#endif

