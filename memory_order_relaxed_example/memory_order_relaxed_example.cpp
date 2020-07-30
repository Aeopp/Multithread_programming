
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
using std::memory_order_relaxed;

void worker(std::atomic<int>* counter)
{
	for (int i = 0; i < 10000; ++i)
	{
		counter->fetch_add(1, memory_order_relaxed);
	}
}

int main()
{
	std::vector<std::thread> threads;

	std::atomic<int> counter(0);

	for (int i = 0; i < 4; ++i)
	{
		threads.push_back(std::thread(worker, &counter));
	}
	for (int i = 0; i < 4; ++i)
	{
		threads[i].join();
	}
	std::cout << "Counter : " << counter << std::endl;

}