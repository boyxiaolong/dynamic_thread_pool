#include "task_callback.h"
#include <string>
#include <atomic>

static std::atomic<int> index(0);

task_callback* task_callback::create(task_callback_fun fun, void* cb_arg)
{
	task_callback* pt = new task_callback(fun, cb_arg);
	pt->set_index(index++);
	printf("create index:%d\n", pt->get_index());
	return pt;
}

task_callback::task_callback(task_callback_fun fun, void* cb_arg)
	: cb_(fun)
	, cb_arg_(cb_arg)
	, task_index_(0)
{
}

task_callback::~task_callback()
{
	printf("~task_callback %d\n", task_index_);
	if (cb_arg_)
	{
		delete cb_arg_;
	}
}

void task_callback::process()
{
	if (cb_)
	{
		cb_(cb_arg_);
	}
}
