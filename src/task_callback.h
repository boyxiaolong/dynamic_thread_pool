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

#ifndef SRC_TASK_CALLBACK_H
#define SRC_TASK_CALLBACK_H

#include <functional>

typedef std::function<void(void*)> task_callback_fun;

class task_callback
{
public:
	static task_callback* create(task_callback_fun fun, void* cb_arg);
	explicit task_callback(task_callback_fun fun, void* cb_arg);
	virtual ~task_callback();

	virtual void process();

	void set_index(int index) { task_index_ = index; }
	int get_index() { return task_index_; }

private:
	task_callback_fun cb_ = NULL;
	void* cb_arg_ = NULL;
	int task_index_;
};
typedef task_callback* ptask_callback;

#endif