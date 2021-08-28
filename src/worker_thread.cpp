#include "worker_thread.h"
#include "task_callback.h"
#include "stdio.h"
#include "thread_pool.h"
#include "utils.h"

worker_thread::worker_thread(int thread_id, thread_pool* pthread_pool) :thread_id_(thread_id)
, thread_runing_(false)
, pthread_pool_(pthread_pool)
{
}

worker_thread::~worker_thread()
{
	printf("~worker_thread %d \n", thread_id_);
	if (thd_)
	{
		thd_->join();
		delete thd_;
	}
}

void worker_thread::run()
{
	while (thread_runing_)
	{
		task_callback_type ptask = pthread_pool_->get_task();
		if (nullptr == ptask)
		{
			break;
		}
		printf("thread %d process\n", thread_id_);
		ptask->process();
	}
}

void worker_thread::start()
{
	if (NULL != thd_)
	{
		return;
	}
	printf("start thread %d\n", thread_id_);
	thread_runing_ = true;
	thd_ = new std::thread(&worker_thread::run, this);
}

void worker_thread::stop()
{
	thread_runing_ = false;
}

int worker_thread::get_thread_id()
{
	return thread_id_;
}

