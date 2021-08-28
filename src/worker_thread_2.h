#ifndef worker_thread__2_h
#define worker_thread__2_h
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

class task_callback;
class thread_pool_2;
class worker_thread_2
{
public:
	worker_thread_2(int thread_id, thread_pool_2* pthread_pool);
	virtual ~worker_thread_2();
	void run();
	void start();
	void stop();

	int get_thread_id();

private:
	std::atomic<bool> thread_runing_;
	std::thread* thd_ = NULL;
	int thread_id_;
	thread_pool_2* pthread_pool_;
};
typedef worker_thread_2* pworker_thread_2;
#endif

