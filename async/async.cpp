#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <atomic>
 volatile int value{ 0 };
 std::atomic<bool> atomic_bool{ true };
 bool flag{ true };

 void my_lock()
 {
	 while (true)
	 {
		 if (atomic_bool)
		 {
			 atomic_bool = false;
			 //atomic_bool.exchange(true, std::memory_order::memory_order_release);
			 return;
		 }
	 }
 }
 void my_unlock()
 {
	 while (true)
	 {
		 if (!atomic_bool)
		 {
			 atomic_bool = true;
			// atomic_bool.exchange(true, std::memory_order::memory_order_release);
			 return;
		 }
	 }
 }
 void foo()
 {
	 for (int i = 0; i < 10000; ++i)
	 {
		 my_lock();
		 ++value;
		 my_unlock();
	 }
		 
 };

void _future()
{
	std::vector<std::future<void>> futures;

	for (int i = 0; i < 10; ++i)
	{
		auto&& _promise = std::async(std::launch::deferred, foo);
		futures.push_back(std::move(_promise));
	}

	for (auto& _promise : futures)
	{
		_promise.get();
	}
	std::cout << value << std::endl;
}
void _thread()
{
	std::vector<std::thread> threads;

	for (int i = 0; i < 10; ++i)
	{
		threads.push_back(std::move(std::thread(foo)));
	}
	for (auto& thread : threads)
	{
		thread.join();
	}
	std::cout << value << std::endl;
};

int main()
{
	_thread();
}