#ifndef task_callback_h
#define task_callback_h

#include <functional>

typedef std::function<void(void*)> task_callback_fun;

class task_callback
{
public:
	explicit task_callback(task_callback_fun fun, void* cb_arg);
	virtual ~task_callback(){}

	virtual void process()
	{
		if (cb_)
		{
			cb_(cb_arg_);
		}

		if (cb_arg_)
		{
			delete cb_arg_;
		}
	}

private:
	task_callback_fun cb_ = NULL;
	void* cb_arg_ = NULL;;
};

#endif