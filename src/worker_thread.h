#ifndef worker_thread_h
#define worker_thread_h
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

class task_callback;
class thread_pool;
class worker_thread
{
public:
	worker_thread(int thread_id, thread_pool* pthread_pool);
	virtual ~worker_thread();
	void run();
	void start();
	void stop();

	int get_thread_id();

private:
	std::atomic<bool> thread_runing_;
	std::thread* thd_ = NULL;
	int thread_id_;
	thread_pool* pthread_pool_;
};
typedef worker_thread* pworker_thread_2;
#endif

