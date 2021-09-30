#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <atomic>
#include <csignal>
#include <atomic>
#include <vector>
#include <list>
#include <string>
#include <coroutine>
#include <map>

#include "task_callback.h"
#include "worker_thread.h"
#include "stdio.h"
#include <memory>
#include "thread_pool.h"

volatile std::sig_atomic_t gSignalStatus;
std::atomic_bool is_running(true);
void sig_handler(int sig)
{
	is_running = false;
}


struct Session
{
	int session_id = 0;
	std::coroutine_handle<> h;
};


struct Awaiter {
	bool await_ready() {
		std::cout << "await ready or not" << std::endl;
		return false;
	}

	void await_resume() {
		std::cout << "await resumed" << std::endl;
	}

	void await_suspend(std::coroutine_handle<> h) {
		std::cout << "await suspended" << std::endl;
		if (ps)
		{
			ps->h = h;
		}
	}
	Session* ps;
};

struct Promise {
	struct promise_type {
		auto get_return_object() noexcept {
			std::cout << "get return object" << std::endl;
			return Promise();
		}

		auto initial_suspend() noexcept {
			std::cout << "initial suspend, return never" << std::endl;
			return std::suspend_never{};
		}

		auto final_suspend() noexcept {
			std::cout << "final suspend, return never" << std::endl;
			return std::suspend_never{};
		}

		void unhandled_exception() {
			std::cout << "unhandle exception" << std::endl;
			std::terminate();
		}

		void return_void() {
			std::cout << "return void" << std::endl;
			return;
		}
	};
};

struct Rpc
{
	Session* ps = nullptr;

};

using session_map = std::map<int, Session*>;
session_map my_ss_map;

void add_to_map(Session* ps)
{
	my_ss_map[ps->session_id] = ps;
}

Session* get_ss(int id)
{
	session_map::iterator iter = my_ss_map.find(id);
	if (my_ss_map.end() == iter)
	{
		return nullptr;
	}

	return iter->second;
}

int gen_session_id()
{
	static int session_id = 0;
	++session_id;
	return session_id;
}

Promise BeginSession() 
{
	Session* ps = new Session;
	ps->session_id = gen_session_id();
	std::cout << "before co_await" << std::endl;
	add_to_map(ps);
	co_await Awaiter{ps};
	co_return;
}



static void handle_input(void* para)
{
	if (nullptr == para)
	{
		return;
	}

	std::string& str = *static_cast<std::string*>(para);
	if (str == "begin")
	{

	}
	std::cout << "get str:" << str << std::endl;
}

int main()
{
	std::signal(SIGINT, sig_handler);
	int thread_num = 1;
	thread_pool tp(thread_num);
	tp.start();

	while (is_running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		printf("pls input:\n");
		std::string input;
		while (std::cin >> input)
		{
			std::cout << "input " << input << std::endl;
			std::string* pstr(new std::string(input));
			std::shared_ptr<task_callback> pdata(task_callback::create(handle_input, pstr));
			tp.push(pdata);
		}
	}
	printf("main exit\n");
	return 0;
}