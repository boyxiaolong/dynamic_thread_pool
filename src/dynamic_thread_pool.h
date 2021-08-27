#ifndef dynamic_thread_pool_h
#define dynamic_thread_pool_h
#include <mutex>
#include <list>
#include <atomic>
#include <memory>
class task_callback;
class worker_thread;

class dynamic_thread_pool
{
public:
	typedef std::list<worker_thread*> thread_vec;
	dynamic_thread_pool(int min_thread_num, int max_thread_num, int max_queue_size);
	~dynamic_thread_pool();

	void start();
	void stop();

	void wait();

	bool push(std::shared_ptr<task_callback> t);

	bool handle_task(std::shared_ptr<task_callback> t);

private:
	bool check_min_threads();
	bool shrink_threads();

private:
	int min_thread_num_;
	int max_thread_num_;
	int max_queue_size_;
	thread_vec thd_vec_;
	std::mutex thread_lock_;
	std::atomic<bool> is_runing_;
};

#endif

