#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <string>
#include <string_view>
#include <queue>
#include <chrono>
#include <random>
#include <functional>
#include <algorithm>
#include <execution>
#include <sstream>
using namespace std::chrono_literals;
using uint32 = unsigned int; 
static constexpr uint32 min_work_milisec = 100;
static constexpr auto thread_sleep_time = 100ms;
static constexpr uint32 thread_num = 100;

// 생산자는 다운로드 페이지를 한다.
// 생산자가 다운로드 페이지를 끝낸 이후 Job 를 Queue 에 Push 한다.
// 소비자는 Queue 에 Job 이 있는지 확인한다.
// Job 이 있다면 해당 Job 를 수행한다.

void Task(std::thread::id id) {
	std::this_thread::sleep_for(std::chrono::milliseconds(thread_sleep_time));
	std::wstringstream wss;
	wss<< id << L" <= thread working ! " << std::endl;
	std::wcout << wss.str();
};

void producer(std::queue<std::thread::id>* job_queue,std::mutex* m,
	std::condition_variable*  cv) 
{
  // 네트워크에서 Job 를 다운로드 받는다.
	 uint32 count = 0;

	while (true)
	{
		if (count >= 10000)return;

		std::this_thread::sleep_for(std::chrono::milliseconds(thread_sleep_time));

		std::thread::id thread_id = std::this_thread::get_id();

		// Critical section  =======================
		{
			std::lock_guard<std::mutex> _lock(*m);
			job_queue->push(std::move(thread_id));
			// 일해라 !!
			cv->notify_one();
			++count;
		}
		// =========================================
	}
};

void consumer(std::queue<std::thread::id>* job_queue, std::mutex* m,
	std::condition_variable* cv)
{
	auto pred{ [&job_queue]() {
			return !job_queue->empty(); } };

	while(true)
	{
		std::unique_lock _lock(*m);
		// 일이 생길때 까지 해당 쓰레드 Sleep 
		// 통보받으면 조건 한번 더 검사하고 기상한다.
		cv->wait(_lock,pred);
		std::thread::id thread_id = job_queue->front();
		job_queue->pop();
		_lock.unlock();

		Task(thread_id);
	}
};

int main(int argc, int* argv[])
{
	std::queue<std::thread::id> job_queue;
	std::mutex m;
	std::condition_variable cv;

	auto make{ [&](auto dummy) {
		using func_type = std::function<void(
		std::queue<std::thread::id>* , std::mutex* ,
		std::condition_variable*)>;

		func_type func_ptr;

		if (dummy == consumer)
			func_ptr = consumer;
		else if (dummy == producer)
			func_ptr = producer;

		return std::thread(func_ptr,&job_queue,&m,&cv);
	} };

	
	std::vector<std::thread> producers;
	std::vector<std::thread> consumers;
	producers.reserve(thread_num);
	consumers.reserve(thread_num);

	for (uint32 i = 0; i < thread_num; ++i)
	{
		producers.push_back(make(producer));
		consumers.push_back(make(consumer));
	};


	auto thread_join{
		[](std::vector<std::thread>& threads) {
		std::for_each
		(std::execution::par, std::begin(threads), std::end(threads),
			[](std::thread& _thread) {
				_thread.join();
			});
	}};

	thread_join(producers);
	cv.notify_all();
	thread_join(consumers);
}
