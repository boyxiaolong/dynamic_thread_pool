#ifndef task_callback_h
#define task_callback_h

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