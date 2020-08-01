#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>
#include <queue>
#include <unordered_map>
#include <any>

using job_type = std::function<std::any()>;

static constexpr uint32_t thread_pool_size = 16;

std::condition_variable condition_value{};
std::queue<job_type> job_queue{};
std::mutex m;
std::vector<std::thread> pool;
			// equal uint32_t
using map_type = std::pair<_Thrd_id_t, std::pair<std::any, std::type_info>>;

std::unordered_map<_Thrd_id_t, std::pair<std::any,std::type_info>> hash_table;

bool predicate()
{
	return !job_queue.empty();
};

void work()
{
	job_type job;

	while (true)
	{
		{
			std::unique_lock<std::mutex> _lock(m);
			condition_value.wait(_lock, predicate);
			job = job_queue.front();
			job_queue.pop();
		}

		job();
		auto id =std::this_thread::get_id();
	}
};

// thread safe
template<typename fn,typename ...args>
void task_add(fn func,args&&... params)
{
	auto&& func_package
	{ 
		[=]()->auto
		{
			return func(params...);
		}  
	};

	{
		std::lock_guard<std::mutex> _lock(m);
		job_queue.push(std::move(func_package));
		
		condition_value.notify_one();
	}
}

bool foo3(int,int,int)
{
	for(int i=0;i<100;++i)

	std::cout << __FUNCTION__ << std::endl;
	return true;
}
bool foo2(int,int)
{
	for (int i = 0; i < 100; ++i)

	std::cout << __FUNCTION__ << std::endl;
	return false;
}
bool foo1(int)
{
	for (int i = 0; i < 100; ++i)
		std::cout << __FUNCTION__ << std::endl;
	return true;
};

bool foo4(int, int, int, int)
{

	for (int i = 0; i < 100; ++i)
		std::cout << __FUNCTION__ << std::endl;
	return true;
}

int main()
{
	pool.reserve(thread_pool_size);

	for (int i = 0; i < thread_pool_size; ++i)
	{
		pool.emplace_back(work);
		pool[i].detach();
	}

	task_add(foo1, 1);
	task_add(foo2, 1, 2);
	task_add(foo3, 1, 2, 3);
	task_add(foo4, 1, 2, 3, 4);
	task_add(foo1, 1);
	task_add(foo2, 1, 2);
	task_add(foo3, 1, 2, 3);
	task_add(foo4, 1, 2, 3, 4);
	task_add(foo1, 1);
	task_add(foo2, 1, 2);
	task_add(foo3, 1, 2, 3);
	task_add(foo4, 1, 2, 3, 4);

	std::this_thread::sleep_for(std::chrono::seconds(100000));
}

