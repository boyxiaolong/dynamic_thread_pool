#include "task_callback.h"

task_callback::task_callback(task_callback_fun fun, void* cb_arg)
	: cb_(fun)
	, cb_arg_(cb_arg)
{

}
