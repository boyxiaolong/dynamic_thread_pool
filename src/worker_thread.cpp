#include "worker_thread.h"
#include "task_callback.h"
#include "stdio.h"
worker_thread::worker_thread(int thread_id, int max_queue_size) :thread_id_(thread_id)
, max_queue_size_(max_queue_size)
, task_size_(0)
, thread_runing_(false)
{
	printf("max_queue_size_ %d\n", max_queue_size_);
}

worker_thread::~worker_thread()
{
	if (thd_)
	{
		delete thd_;
	}
	printf("~worker_thread %d \n", thread_id_);
}

void worker_thread::run()
{
	while (thread_runing_)
	{
		std::queue<std::shared_ptr<task_callback>> temp;
		{
			std::unique_lock<std::mutex> guard(task_lock_);
			if (!tasks_.empty())
			{
				std::swap(temp, tasks_);
			}
		}
		{
			std::unique_lock<std::mutex> guard(thread_lock_);
			if (temp.empty() && thread_runing_)
			{
				con_.wait(guard, [] {std::chrono::milliseconds(10); return true; });
				continue;
			}
		}
		while (!temp.empty())
		{
			std::shared_ptr<task_callback> t = temp.front();
			temp.pop();
			t->process();
			--task_size_;
		}
	}
}

void worker_thread::start()
{
	if (NULL != thd_)
	{
		return;
	}
	thread_runing_ = true;
	thd_ = new std::thread(&worker_thread::run, this);
}

bool worker_thread::push(std::shared_ptr<task_callback> data)
{
	bool is_notify = false;
	{
		std::unique_lock<std::mutex> guard(task_lock_);
		int total_size = tasks_.size();
		if (total_size >= max_queue_size_)
		{
			printf("max size\n");
			return false;
		}

		is_notify = total_size == 0;
		tasks_.push(data);
		++task_size_;
	}
	if (is_notify)
	{
		std::unique_lock<std::mutex> guard(thread_lock_);
		con_.notify_one();
	}
	return true;
}

void worker_thread::wait()
{
	if (thd_)
	{
		thd_->join();
	}
}

void worker_thread::stop()
{
	thread_runing_ = false;
	wait();
	printf("thread %d stop\n", thread_id_);
}

bool worker_thread::is_full()
{
	return task_size_ >= max_queue_size_;
}

bool worker_thread::is_empty()
{
	return task_size_ < 1;
}

int worker_thread::get_thread_id()
{
	return thread_id_;
}

