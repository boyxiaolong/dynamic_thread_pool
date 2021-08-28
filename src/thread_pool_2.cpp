#include "thread_pool_2.h"
#include "worker_thread_2.h"
#include "stdio.h"

thread_pool_2::thread_pool_2(int min_thread_num, int max_thread_num, int max_queue_size) : min_thread_num_(min_thread_num)
, max_thread_num_(max_thread_num)
, max_queue_size_(max_queue_size)
, is_runing_(true)
{

}

thread_pool_2::~thread_pool_2()
{
	stop();
}

void thread_pool_2::start()
{
	check_min_threads();
}

void thread_pool_2::stop()
{
	printf("stop threadpool\n");
	is_runing_ = false;
	std::unique_lock<std::mutex> guard(thread_lock_);
	for (thread_vec::iterator iter = thd_vec_.begin();
		iter != thd_vec_.end(); ++iter)
	{
		pworker_thread_2 pw = *iter;
		if (pw)
		{
			pw->stop();
		}
	}

	task_con_.notify_all();

	for (thread_vec::iterator iter = thd_vec_.begin();
		iter != thd_vec_.end(); ++iter)
	{
		pworker_thread_2 pw = *iter;
		if (pw)
		{
			delete pw;
		}
	}
}

bool thread_pool_2::push(task_callback* t)
{
	if (!is_runing_)
	{
		return false;
	}
	return handle_task(t);
}

bool thread_pool_2::handle_task(task_callback* t)
{
	std::unique_lock<std::mutex> guard(task_lock_);
	bool is_empty = tasks_.empty();
	tasks_.push(t);
	if (is_empty)
	{
		task_con_.notify_one();
	}
	return true;
}


task_callback* thread_pool_2::get_task()
{
	std::unique_lock<std::mutex> guard(task_lock_);
	while (tasks_.empty())
	{
		task_con_.wait(guard);
	}
	
	auto pt = tasks_.front();
	tasks_.pop();
	return pt;
}

void thread_pool_2::wait_tasks()
{
	std::unique_lock<std::mutex> guard(thread_lock_);
	task_con_.wait(guard);
}

bool thread_pool_2::check_min_threads()
{
	std::unique_lock<std::mutex> guard(thread_lock_);
	int thd_size = thd_vec_.size();
	if (thd_size < min_thread_num_)
	{
		auto thread_fun = []() {};
		for (int i = thd_size + 1; i <= min_thread_num_; ++i)
		{
			pworker_thread_2 pwt = new worker_thread_2(i, this);
			pwt->start();
			thd_vec_.push_back(pwt);
		}
		return true;
	}
	return false;
}

bool thread_pool_2::shrink_threads()
{
	return true;
}
