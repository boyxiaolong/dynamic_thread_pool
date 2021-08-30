#include "thread_pool.h"
#include "worker_thread.h"
#include "stdio.h"
#include "task_callback.h"

thread_pool::thread_pool(int thread_num) : thread_num_(thread_num)
, is_runing_(true)
, total_task_size_(0)
{

}

thread_pool::~thread_pool()
{
	stop();
}

void thread_pool::start()
{
	_gen_threads();
}

void thread_pool::stop()
{
	printf("stop threadpool\n");
	is_runing_ = false;
	{
		std::unique_lock<std::mutex> guard(thread_lock_);
		for (thread_vec_type::iterator iter = thd_vec_.begin();
			iter != thd_vec_.end(); ++iter)
		{
			worker_thread_type pw = *iter;
			if (pw)
			{
				pw->stop();
			}
		}

		task_con_.notify_all();
	}

	printf("left task :%d\b", (int)total_task_size_);

	std::unique_lock<std::mutex> guard(task_lock_);
	printf("queue size:%d\b", (int)tasks_.size());
	while (!tasks_.empty())
	{
		task_callback_type pc = tasks_.front();
		tasks_.pop();
		printf("dtor task index:%d\n", pc->get_index());
	}
}

bool thread_pool::push(task_callback_type t)
{
	if (!is_runing_)
	{
		return false;
	}
	return handle_task(t);
}

bool thread_pool::handle_task(task_callback_type t)
{
	++total_task_size_;
	std::unique_lock<std::mutex> guard(task_lock_);
	bool is_empty = tasks_.empty();
	tasks_.push(t);
	if (is_empty)
	{
		task_con_.notify_one();
	}
	return true;
}


task_callback_type thread_pool::get_task()
{
	std::unique_lock<std::mutex> guard(task_lock_);
	while (tasks_.empty())
	{
		if (!is_runing_)
		{
			return NULL;
		}
		task_con_.wait(guard);
	}
	
	task_callback_type pt = tasks_.front();
	tasks_.pop();

	--total_task_size_;
	return pt;
}

void thread_pool::wait_tasks()
{
	std::unique_lock<std::mutex> guard(thread_lock_);
	task_con_.wait(guard);
}

bool thread_pool::_gen_threads()
{
	std::unique_lock<std::mutex> guard(thread_lock_);
	size_t thd_size = thd_vec_.size();
	if (thd_size < thread_num_)
	{
		auto thread_fun = []() {};
		for (size_t i = thd_size + 1; i <= thread_num_; ++i)
		{
			worker_thread_type pwt(new worker_thread(i, this));
			pwt->start();
			thd_vec_.push_back(pwt);
		}
		return true;
	}
	return false;
}