#ifndef dynamic_thread_pool_2_h
#define dynamic_thread_pool_2_h
#include <list>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

class task_callback;
class worker_thread_2;

class thread_pool_2
{
public:
	typedef std::list<worker_thread_2*> thread_vec;
	thread_pool_2(int min_thread_num, int max_thread_num, int max_queue_size);
	~thread_pool_2();

	void start();
	void stop();

	bool push(task_callback* t);

	bool handle_task(task_callback* t);

	task_callback* get_task();
	void wait_tasks();

private:
	bool check_min_threads();

private:
	int min_thread_num_;
	thread_vec thd_vec_;
	std::mutex thread_lock_;
	std::atomic<bool> is_runing_;

	std::mutex task_lock_;
	std::queue<task_callback*> tasks_;
	std::condition_variable task_con_;

};

#endif

