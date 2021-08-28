#include "worker_thread_2.h"
#include "task_callback.h"
#include "stdio.h"
#include "thread_pool_2.h"
#include "unils.h"

worker_thread_2::worker_thread_2(int thread_id, thread_pool_2* pthread_pool) :thread_id_(thread_id)
, thread_runing_(false)
, pthread_pool_(pthread_pool)
{
}

worker_thread_2::~worker_thread_2()
{
	printf("~worker_thread %d \n", thread_id_);
	if (thd_)
	{
		thd_->join();
		delete thd_;
	}
}

void worker_thread_2::run()
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

void worker_thread_2::start()
{
	if (NULL != thd_)
	{
		return;
	}
	printf("start thread %d\n", thread_id_);
	thread_runing_ = true;
	thd_ = new std::thread(&worker_thread_2::run, this);
}

void worker_thread_2::stop()
{
	thread_runing_ = false;
}

int worker_thread_2::get_thread_id()
{
	return thread_id_;
}

