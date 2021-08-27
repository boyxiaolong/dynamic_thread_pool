#include "task_callback.h"
#include <string>

task_callback::task_callback(task_callback_fun fun, void* cb_arg)
	: cb_(fun)
	, cb_arg_(cb_arg)
{
}

task_callback::~task_callback()
{
	printf("~task_callback\n");
	if (cb_arg_)
	{
		delete cb_arg_;
	}
}
