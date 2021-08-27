#include "task_callback.h"
#include <string>

task_callback::task_callback(task_callback_fun fun, void* cb_arg)
	: cb_(fun)
	, cb_arg_(cb_arg)
{
	std::string& str = *(static_cast<std::string*> (cb_arg_));
}

task_callback::~task_callback()
{
	printf("~task_callback\n");
	if (cb_arg_)
	{
		std::string& str = *(static_cast<std::string*> (cb_arg_));
		delete cb_arg_;
	}
}
