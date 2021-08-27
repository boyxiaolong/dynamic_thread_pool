#ifndef worker_thread_h
#define worker_thread_h
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>

class task_callback;
class worker_thread
{
public:
	worker_thread(int thread_id, int max_queue_size);
	virtual ~worker_thread();
	void run();
	void start();
	bool push(std::shared_ptr<task_callback> data);
	void wait();
	void stop();
	bool is_full();
	bool is_empty();
	int get_thread_id();

private:
	std::queue<std::shared_ptr<task_callback>> tasks_;
	std::atomic<bool> thread_runing_;
	std::mutex task_lock_;
	std::mutex thread_lock_;
	std::condition_variable con_;
	int max_queue_size_;
	std::thread* thd_ = NULL;
	int thread_id_;
	std::atomic<int> task_size_;
};
typedef worker_thread* pworker_thread;
#endif

