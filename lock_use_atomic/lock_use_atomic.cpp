//문제 1
//std::atomic<bool> 을 사용해서 lock() 과 unlock() 을 만들어보세요.
//참고로 compare_exchange_strong 함수를 사용하는 것이 도움이 됩니다.
//compare_exchange_strong 은 아래와 같이 생겼습니다.
//
//C / C++ 확대 축소
//bool compare_exchange_strong(
//	T & expected, T desired,
//	std::memory_order order = std::memory_order_seq_cst);
//만일 현재 atomic 객체의 값이 expected 와 같다면 
//desired 로 바꾸고 true 를 리턴합니다.expected 와 다르다면 
//desired 로 바꾸지 않고 그냥 false 를 리턴합니다.물론 이 읽기 - 
//수정하기 - 쓰기 명령은 atomic 하게 실행됩니다.
#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#include <fstream>
#include <chrono>
#include <sstream>
// static std::atomic<bool> atom_bool{ true };
static std::atomic_flag atom_bool;

static std::atomic_int32_t atom_int{ 0l };
static constexpr uint32_t for_loop_num = 10000;
static std::mutex _mutex;
static std::ofstream file("log.txt");

static  uint32_t target{ 0l };

static void lock()
{
	auto id = std::this_thread::get_id();

	static bool _true{ true };

	while (true) 
	{
		if (true == atom_bool.test_and_set(std::memory_order::memory_order_seq_cst))
			return;
	}
};

static void unlock()
{
	auto id = std::this_thread::get_id();

	static bool _false { false };

	
	if (false == atom_bool.test_and_set(std::memory_order::memory_order_seq_cst))
		return;
};

static void foo()
{
	auto id = std::this_thread::get_id();
	
	for (uint32_t i = 0; i < for_loop_num; ++i)
	{
		lock();
		++target;
		 unlock();
	}
}

int main(int argc,int * argv[])
{
	constexpr uint32_t thread_num = 3;
	constexpr uint32_t result = thread_num * for_loop_num;
	std::vector<std::thread> vec_thread;
	vec_thread.reserve(thread_num);

	for (uint32_t i = 0; i < thread_num; ++i)
	{
		vec_thread.push_back(std::thread(foo));
	}

	std::for_each(std::begin(vec_thread), std::end(vec_thread), [](std::thread& _thread) {
		_thread.join();});

	std::cout << "result : " << target << " answer : " << result;

}