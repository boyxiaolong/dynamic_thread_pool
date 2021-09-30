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

Promise BeginSession(int ss_id) 
{
	printf("begin ssid:%d\n", ss_id);
	Session* ps = new Session;
	ps->session_id = ss_id;
	std::cout << "before co_await" << std::endl;
	add_to_map(ps);
	co_await Awaiter{ps};

	std::cout << "ssid : " << ps->session_id << " handle end\n";
	delete ps;
	co_return;
}

void EndSession(int ss_id)
{
	printf("end ssid:%d\n", ss_id);
	Session* ps = get_ss(ss_id);
	if (nullptr == ps)
	{
		return;
	}

	ps->h.resume();
}

struct input_info
{
	int handle_id;
	int session_id;
};

enum handle_enum
{
	handle_input_enum = 1,
	handle_output_enum = 2
};

static void handle_input(void* para)
{
	if (nullptr == para)
	{
		return;
	}

	input_info& info = *static_cast<input_info*>(para);
	if (info.handle_id == handle_input_enum)
	{
		BeginSession(info.session_id);
	}
	else if (info.handle_id == handle_output_enum)
	{
		EndSession(info.session_id);
	}
	std::cout << "ss:" << info.session_id;
}

int main()
{
	std::signal(SIGINT, sig_handler);
	int thread_num = 1;
	thread_pool tp(thread_num);
	tp.start();
	int last_id = 0;
	while (is_running)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		printf("pls input:\n");
		std::string input;
		
		while (std::cin >> input)
		{
			std::cout << "input " << input << std::endl;
			
			input_info* pinfo = new input_info;
			if (input == "begin")
			{
				last_id = gen_session_id();
				pinfo->handle_id = handle_input_enum;
				pinfo->session_id = last_id;
			}
			else if (input == "end")
			{
				pinfo->session_id = last_id;
				pinfo->handle_id = handle_output_enum;
			}
			else
			{
				printf("error\n");
				continue;
			}
			std::string* pstr(new std::string(input));
			std::shared_ptr<task_callback> pdata(task_callback::create(handle_input, pinfo));
			tp.push(pdata);
		}
	}
	printf("main exit\n");
	return 0;
}